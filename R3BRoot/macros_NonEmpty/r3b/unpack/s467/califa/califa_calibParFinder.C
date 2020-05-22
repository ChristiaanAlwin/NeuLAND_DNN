/* Additional info:
 *
 * One needs to set up the 2020 experiments s444 and s467, unpackers are:
 *
 * at $UCESB_DIR/../upexps/202002_s444 and $UCESB_DIR/../upexps/202002_s467
 *
 * Before executing the macro, set up the paths to lmd files, upexps and
 * califa mapping file "califamapfilename". Also set up the name of output
 * file "outputCalFile"
 *
 * @since January 18th, 2020
 */

typedef struct EXT_STR_h101_t
{
    EXT_STR_h101_unpack_t unpack;
    EXT_STR_h101_CALIFA_t califa;
} EXT_STR_h101;

void califa_calibParFinder()
{
    TStopwatch timer;
    timer.Start();

    const Int_t nev = -1; /* number of events to read, -1 - until CTRL+C */

    /* Create source using ucesb for input ------------------ */
    // TString filename = "--stream=lxg0898:6002";
    TString filename = "~/lmd/califa2020/2020-01-19_11-19_0002.lmd";
    TString outputFileName = "./califa_calpar_0001.root";

    /* Create source using ucesb for input ------------------ */
    // UCESB paths
    TString ntuple_options = "RAW";
    TString ucesb_dir = getenv("UCESB_DIR");
    TString ucesb_path = ucesb_dir + "/../upexps/201911_eng2/201911_eng2";
    ucesb_path.ReplaceAll("//", "/");

    // Parameters for CALIFA
    TString dir = gSystem->Getenv("VMCWORKDIR");
    TString califamapdir = dir + "/sofia/macros/s444/parameters/";
    TString califamapfilename = califamapdir + "CALIFA_mapping.par";
    califamapfilename.ReplaceAll("//", "/");

    // CALIFA output file with parameters for calibrating in keV
    TString outputCalFile = "./Califa_CalibParamFeb2020.root";

    /* Definition of reader --------------------------------- */
    EXT_STR_h101 ucesb_struct;

    R3BUcesbSource* source =
        new R3BUcesbSource(filename, ntuple_options, ucesb_path, &ucesb_struct, sizeof(ucesb_struct));
    source->SetMaxEvents(nev);

    source->AddReader(new R3BUnpackReader((EXT_STR_h101_unpack*)&ucesb_struct, offsetof(EXT_STR_h101, unpack)));
    source->AddReader(
        new R3BCalifaFebexReader((EXT_STR_h101_CALIFA*)&ucesb_struct.califa, offsetof(EXT_STR_h101, califa)));

    /* Create online run ------------------------------------ */
    FairRunOnline* run = new FairRunOnline(source);
    run->SetRunId(1);
    run->SetSink(new FairRootFileSink(outputFileName));

    /* Add analysis task ------------------------------------ */

    // R3BCalifaMapped2CrystalCalPar ----
    TArrayF* EnergythePeaks = new TArrayF();
    Float_t e1 = 1332.5;
    Float_t e2 = 1173.2;
    EnergythePeaks->Set(2);
    EnergythePeaks->AddAt(e1, 0);
    EnergythePeaks->AddAt(e2, 1);

    R3BCalifaMapped2CrystalCalPar* CalPar = new R3BCalifaMapped2CrystalCalPar();
    CalPar->SetMinStadistics(1000);
    CalPar->SetNumParameterFit(2); // OPTIONAL by default 2
    // Gamma range
    CalPar->SetCalRange_left(500);
    CalPar->SetCalRange_right(2000);
    CalPar->SetCalRange_bins(130);
    // particle range
    CalPar->SetCalRangeP_left(80);
    CalPar->SetCalRangeP_right(180);
    CalPar->SetCalRangeP_bins(100);
    CalPar->SetSigma(3.0);
    CalPar->SetThreshold(0.0001);
    CalPar->SetEnergyPeaks(EnergythePeaks);
    run->AddTask(CalPar);

    /* Runtime data base ------------------------------------ */
    FairRuntimeDb* rtdb = run->GetRuntimeDb();
    // CALIFA mapping
    FairParAsciiFileIo* parIo1 = new FairParAsciiFileIo(); // Ascii
    parIo1->open(califamapfilename, "in");
    rtdb->setFirstInput(parIo1);
    rtdb->print();

    /* Initialize ------------------------------------------- */
    run->Init();
    //    FairLogger::GetLogger()->SetLogScreenLevel("WARNING");
    //    FairLogger::GetLogger()->SetLogScreenLevel("DEBUG");
    FairLogger::GetLogger()->SetLogScreenLevel("INFO");

    // Choose Root or Ascii file
    // 1-Root file with the Calibration Parameters
    Bool_t kParameterMerged = kTRUE;
    FairParRootFileIo* parOut = new FairParRootFileIo(kParameterMerged);
    parOut->open(outputCalFile);
    rtdb->setOutput(parOut);
    // 2-Ascii file with the Calibration Parameters
    /*FairParAsciiFileIo* parIo1 = new FairParAsciiFileIo();
      parIo1->open("Califa_CalibParam.par","out");
      rtdb->setOutput(parIo1);
      */

    /* Run -------------------------------------------------- */
    run->Run((nev < 0) ? nev : 0, (nev < 0) ? 0 : nev);

    /* Save parameters (if needed) -------------------------- */
    rtdb->saveOutput();

    /* Finish ----------------------------------------------- */
    timer.Stop();
    Double_t rtime = timer.RealTime();
    Double_t ctime = timer.CpuTime();
    cout << endl << endl;
    cout << "Macro finished succesfully." << endl;
    cout << "Output file is " << outputFileName << endl;
    cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << endl << endl;
    gApplication->Terminate();
}
