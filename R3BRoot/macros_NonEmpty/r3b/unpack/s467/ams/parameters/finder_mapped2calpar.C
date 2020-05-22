/* Additional info:
 *
 * One needs to set up the 2020 experiments s444 and s467, unpackers are:
 *
 * at $UCESB_DIR/../upexps/202002_s444 and $UCESB_DIR/../upexps/202002_s467
 *
 * Before executing the macro, set up the paths to lmd files, upexps and
 * AMS mapping file "amsmapfilename". Also set up the name of output
 * file "outputCalFile" that will contain the pedestal parameters for each ams
 *
 * @since January 23th, 2020
 */

typedef struct EXT_STR_h101_t
{
    EXT_STR_h101_unpack_t unpack;
    EXT_STR_h101_AMS_t ams;
} EXT_STR_h101;

void finder_mapped2calpar()
{
    TStopwatch timer;
    timer.Start();

    // const Int_t nev = -1; /* number of events to read, -1 - until CTRL+C */
    const Int_t nev = -1; /* number of events to read, -1 - until CTRL+C */

    const Int_t expId = 444; // select experiment: 444 or 467

    /* Create source using ucesb for input ------------------ */
    // TString filename = "--stream=lxg0898:6002";
    TString filename = "~/lmd/ams_2019-12-07_21-45_on.lmd";

    // Output file ------------------------------------------
    TString outputFileName = "calparAms_01.root";

    // Parameters for AMS mapping
    TString dir = gSystem->Getenv("VMCWORKDIR");
    TString amsmapdir = dir + "/macros/r3b/unpack/s467/ams/parameters/";
    TString amsmapfilename = amsmapdir + "AMS_mapping.par";
    amsmapfilename.ReplaceAll("//", "/");

    // AMS output file with pedestals and mapping parameters
    TString outputCalFile = "./Ams_pedestals_Feb_2020.root";

    TString ntuple_options = "RAW";
    TString ucesb_dir = getenv("UCESB_DIR");
    /*
      //TString ucesb_path = ucesb_dir + "/../upexps/201911_eng/201911_eng";
      TString ucesb_path = ucesb_dir + "/../upexps/201911_eng2/201911_eng2 --input-buffer=100Mi";
      ucesb_path.ReplaceAll("//","/");
    */

    TString ucesb_path;
    if (expId == 444)
    {
        // ucesb_path = upexps_dir + "/202002_s444/202002_s444 --allow-errors --input-buffer=100Mi";
        ucesb_path = "/u/land/htt/upexps/202002_s444/202002_s444 --allow-errors --input-buffer=100Mi";
    }
    else if (expId == 467)
    {
        // ucesb_path = upexps_dir + "/202002_s467/202002_s467 --allow-errors --input-buffer=100Mi";
        ucesb_path = "/u/land/htt/upexps/202002_s467/202002_s467 --allow-errors --input-buffer=100Mi";
    }
    else
    {
        std::cout << "Experiment was not selected!" << std::endl;
        gApplication->Terminate();
    }
    ucesb_path.ReplaceAll("//", "/");

    // Create source using ucesb for input ------------------
    EXT_STR_h101 ucesb_struct;

    R3BUcesbSource* source =
        new R3BUcesbSource(filename, ntuple_options, ucesb_path, &ucesb_struct, sizeof(ucesb_struct));
    source->SetMaxEvents(nev);

    source->AddReader(new R3BUnpackReader((EXT_STR_h101_unpack*)&ucesb_struct, offsetof(EXT_STR_h101, unpack)));
    source->AddReader(new R3BAmsReader((EXT_STR_h101_AMS*)&ucesb_struct.ams, offsetof(EXT_STR_h101, ams)));

    // Create online run ------------------------------------
    FairRunOnline* run = new FairRunOnline(source);
    run->SetRunId(1);
    run->SetSink(new FairRootFileSink(outputFileName));

    /* Add analysis task ------------------------------------ */
    R3BAmsMapped2StripCalPar* CalPar = new R3BAmsMapped2StripCalPar();
    CalPar->SetNumDetectors(6); // Number of ams detectors
    CalPar->SetNumStrips(1024); // 1024 strips per ams detector
    CalPar->SetNumStripsK(384); // 384 strips for the K-side
    CalPar->SetNumStripsS(640); // 640 strips for the S-side
    CalPar->SetMaxSigma(10);    // Max. sigma to mark dead strips: 5 ADC units
    CalPar->SetMinStadistics(1000);
    CalPar->SetCalRange_left(0);
    CalPar->SetCalRange_right(1000);
    CalPar->SetCalRange_bins(1000);
    CalPar->SetPrintParamsDaq(kTRUE);
    run->AddTask(CalPar);

    /* Runtime data base ------------------------------------ */
    FairRuntimeDb* rtdb = run->GetRuntimeDb();
    // AMS mapping
    FairParAsciiFileIo* parIo1 = new FairParAsciiFileIo(); // Ascii
    parIo1->open(amsmapfilename, "in");
    rtdb->setFirstInput(parIo1);
    rtdb->print();

    /* Initialize ------------------------------------------- */
    run->Init();
    FairLogger::GetLogger()->SetLogScreenLevel("INFO");
    // FairLogger::GetLogger()->SetLogScreenLevel("WARNING");
    // FairLogger::GetLogger()->SetLogScreenLevel("DEBUG");

    // Choose Root or Ascii file
    // 1-Root file with the Calibartion Parameters
    Bool_t kParameterMerged = kTRUE;
    FairParRootFileIo* parOut = new FairParRootFileIo(kParameterMerged);
    parOut->open(outputCalFile);
    rtdb->setOutput(parOut);

    /* Run -------------------------------------------------- */
    run->Run((nev < 0) ? nev : 0, (nev < 0) ? 0 : nev);

    /* Save parameters (if needed) -------------------------- */
    rtdb->saveOutput();

    /* Finish ----------------------------------------------- */
    timer.Stop();
    Double_t rtime = timer.RealTime();
    Double_t ctime = timer.CpuTime();
    Float_t cpuUsage = ctime / rtime;
    cout << "CPU used: " << cpuUsage << endl;
    std::cout << std::endl << std::endl;
    std::cout << "Macro finished succesfully." << std::endl;
    std::cout << "Output file is " << outputFileName << std::endl;
    std::cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << std::endl << std::endl;
    gApplication->Terminate();
}
