/* Additional info:
 * To generate the header file used for the R3BUcesbSource (ext_h101.h), use:
 *
 * ./201902_s444 --ntuple=RAW:SST,id=h101_AMS,ext_h101_ams.h
 *
 * at $UCESB_DIR/upexps/201902_s444
 *
 * Put this header file into the 'r3bsource' directory and recompile.
 *
 * Author: Jose Luis <joseluis.rodriguez.sanchez@usc.es>
 * @since July 22, 2018
 * */

typedef struct EXT_STR_h101_t
{
    EXT_STR_h101_unpack_t unpack;
    EXT_STR_h101_AMS_t ams;
} EXT_STR_h101;

void ams_online()
{
    TStopwatch timer;
    timer.Start();

    // const Int_t nev = -1; /* number of events to read, -1 - until CTRL+C */
    const Int_t nev = -1; /* number of events to read, -1 - until CTRL+C */

    /* Create source using ucesb for input ------------------ */

    TString filename = "--stream=lxir123:7803";
    // TString filename = "~/lmd/ams_r4l-44_2019-09-12.lmd";

    TString outputFileName = "data_online.root";

    TString ntuple_options = "RAW";
    TString ucesb_dir = getenv("UCESB_DIR");
    TString ucesb_path = ucesb_dir + "/../upexps/201911_eng/201911_eng --input-buffer=100Mi";
    ucesb_path.ReplaceAll("//", "/");

    EXT_STR_h101 ucesb_struct;

    R3BUcesbSource* source =
        new R3BUcesbSource(filename, ntuple_options, ucesb_path, &ucesb_struct, sizeof(ucesb_struct));
    source->SetMaxEvents(nev);

    /* Definition of reader --------------------------------- */
    R3BUnpackReader* unpackreader =
        new R3BUnpackReader((EXT_STR_h101_unpack*)&ucesb_struct, offsetof(EXT_STR_h101, unpack));

    R3BAmsReader* unpackams = new R3BAmsReader((EXT_STR_h101_AMS*)&ucesb_struct.ams, offsetof(EXT_STR_h101, ams));

    /* Add readers ------------------------------------------ */
    source->AddReader(unpackreader);
    unpackams->SetOnline(true);
    source->AddReader(unpackams);

    /* Create online run ------------------------------------ */
    FairRunOnline* run = new FairRunOnline(source);
    run->SetRunId(1);
    run->SetSink(new FairRootFileSink(outputFileName));
    Int_t refresh = 1;
    Int_t port = 8886;
    run->ActivateHttpServer(refresh, port);

    /* Runtime data base ------------------------------------ */
    FairRuntimeDb* rtdb = run->GetRuntimeDb();

    /* Load parameters   ------------------------------------ */
    FairParRootFileIo* parIo1 = new FairParRootFileIo();
    parIo1->open("Ams_CalibParam.root", "in");
    rtdb->setFirstInput(parIo1);
    rtdb->print();

    /* Add analysis task ------------------------------------ */
    R3BAmsMapped2StripCal* Map2Cal = new R3BAmsMapped2StripCal();
    Map2Cal->SetOnline(true);
    run->AddTask(Map2Cal);
    R3BAmsStripCal2Hit* Cal2Hit = new R3BAmsStripCal2Hit();
    Cal2Hit->SetOnline(true);
    run->AddTask(Cal2Hit);

    /* Add online task ------------------------------------ */
    R3BAmsOnlineSpectra* online = new R3BAmsOnlineSpectra();
    run->AddTask(online);

    /* Initialize ------------------------------------------- */
    run->Init();
    FairLogger::GetLogger()->SetLogScreenLevel("INFO");
    // FairLogger::GetLogger()->SetLogScreenLevel("WARNING");
    // FairLogger::GetLogger()->SetLogScreenLevel("DEBUG");
    /* ------------------------------------------------------ */

    /* Run -------------------------------------------------- */
    run->Run((nev < 0) ? nev : 0, (nev < 0) ? 0 : nev);

    /* Finish ----------------------------------------------- */
    timer.Stop();
    Double_t rtime = timer.RealTime();
    Double_t ctime = timer.CpuTime();
    std::cout << std::endl << std::endl;
    std::cout << "Macro finished succesfully." << std::endl;
    std::cout << "Output file is " << outputFileName << std::endl;
    std::cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << std::endl << std::endl;
    // gApplication->Terminate();
}
