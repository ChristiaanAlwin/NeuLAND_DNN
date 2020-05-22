/* In order to generate input for this, please go to $UCESB_DIR and run:
 *
 * Additional info:
 * To generate the header file used for the R3BUcesbSource (ext_h101.h), use:
 *
 * $UCESB_DIR/upexps/s438b/s438b --ntuple=UNPACK:TRIGGER,UNPACK:EVENTNO,RAW\
 *     STRUCT_HH,ext_h101.h
 *
 * Put this header file into the 'r3bsource' directory and recompile.
 * */

// extern "C" {
//#include "/home/bloeher/git/R3BRoot/r3bsource/ext_h101_full.h"
// #include "/u/syndikus/R3BRoot/r3bsource/ext_h101.h"
// }

typedef struct EXT_STR_h101_t
{
    //  EXT_STR_h101_LOS_onion_t los;
    EXT_STR_h101_unpack_t unpack;
    EXT_STR_h101_PSP_onion_t psp;
} EXT_STR_h101;

/*
 * To run this macro: root -l -b 'unpack_pspx_online.C()'
 * To change the range for the energy plots of the pspx, go to r3bbase/R3BOnlineSpectra.cxx, change the value Emax in
 * Init() to a propriate number, recompile.
 */

void unpack_pspx1_online(Int_t run_nr = 1, Int_t date = 180821, Int_t detector = 2, Int_t runid = 21)
{
    //    Int_t RunId=100;
    //    TString runNumber = Form("%03d", RunId);
    TStopwatch timer;
    timer.Start();

    const Int_t nev = -1; /* number of events to read, -1 - until CTRL+C */

    /* Create source using ucesb for input ------------------ */

     TString filename = "stream://x86l-23";
     //TString outputFileName = "/tmp/pspx_stream_test.root";
//    TString filename = Form("/home/land/PSPX/lmd_tests/mounted/beam_181128/pspx1_pspx2_run19_000[1-9].lmd", run_nr);
    TString outputFileName = Form("/home/land/PSPX/rootfiles_tests/beam_181205_online/pspx1_online%i.root",run_nr);
    TString ntuple_options = "UNPACK:EVENTNO,UNPACK:TRIGGER,RAW,PSPX";
    // TString ucesb_dir = getenv("UCESB_DIR");
    // TString ucesb_path = ucesb_dir + "/../upexps/jun16Xe/jun16Xe";
//    TString ucesb_path = "/lynx/Lynx/land/usr/land/landexp/kvi2018/upexps/kvi2018/kvi2018_001_060"; //--allow-errors
        //TString ucesb_path = "/home/land/upexps/kvi2018/kvi2018";
        TString ucesb_path = "/home/land/upexps/kvi2018/kvi2018";
    // TString ucesb_path = "/lynx/Lynx/land/usr/land/landexp/201810_s444/upexps/201810_s444/201810_s444";
    // //--allow-errors not yet working, PSPX not yet included

    TString pspxpar_dir = "/home/land/R3BRoot_PSPX/psp/par/";
     TString parPspxMappedFileName = "s444_pspx_mapped.par";
     TString parPspxPrecalFileName = "s444_pspx_precal_default_4det.par";
     TString parPspxCalFileName = "s444_pspx_cal_default_4det.par";
     TString parPspxHitFileName = "s444_pspx_hit_default_4det.par";

    // for data taken with an alpha source during the preparation of the phase0 experiments. only one detector read out
    // with a single filter
   // TString parPspxMappedFileName = "alpha18_pspx_mapped.par";
   // TString parPspxPrecalFileName = Form("alpha18_det%d_pspx_precal.par", detector);
   // TString parPspxCalFileName = Form("alpha18_det%d_pspx_cal.par", detector);
   // TString parPspxHitFileName = "alpha18_pspx_hit_default.par";

    EXT_STR_h101 ucesb_struct;
    R3BUcesbSource* source =
        new R3BUcesbSource(filename, ntuple_options, ucesb_path, &ucesb_struct, sizeof(ucesb_struct));
    source->SetMaxEvents(nev);

    source->AddReader(new R3BUnpackReader((EXT_STR_h101_unpack*)&ucesb_struct.unpack, offsetof(EXT_STR_h101, unpack)));
    source->AddReader(new R3BPspxReader((EXT_STR_h101_PSP*)&ucesb_struct.psp, offsetof(EXT_STR_h101, psp)));

    /* Create online run ------------------------------------ */
    const Int_t refresh = 100; // refresh rate for saving
    FairRunOnline* run = new FairRunOnline(source);
    run->SetRunId(runid);
    run->ActivateHttpServer(refresh,1100);

    // To see the canvases open an internet browser and open localhost:8080 on the same machine or machinename:8080 from
    // within the same network

    // Create analysis run ----------------------------------
    // FairRunAna* run = new FairRunAna();
    run->SetOutputFile(outputFileName.Data());

    /* Runtime data base ------------------------------------ */
    FairRuntimeDb* rtdb1 = run->GetRuntimeDb();
    /*
        Bool_t kParameterMerged = kTRUE;
        FairParRootFileIo* parOut1 = new FairParRootFileIo(kParameterMerged);
        TList *parList = new TList();
        parList->Add(new TObjString("tofd_time_params_run057.root"));
        parList->Add(new TObjString("los_params_25-05-16.root"));
        parList->Add(new TObjString("tofd_hit_params_run054.root"));
        parOut1->open(parList);
        rtdb1->setFirstInput(parOut1);
        */

    FairParAsciiFileIo* parInput = new FairParAsciiFileIo();
    TList* parList = new TList();
    parList->Add(new TObjString(pspxpar_dir + parPspxMappedFileName));
    parList->Add(new TObjString(pspxpar_dir + parPspxPrecalFileName));
    parList->Add(new TObjString(pspxpar_dir + parPspxCalFileName));
    parList->Add(new TObjString(pspxpar_dir + parPspxHitFileName));
    parInput->open(parList);
    rtdb1->setFirstInput(parInput);
    rtdb1->print();

    /* Create ALADIN field map ------------------------------ */
    /*    R3BAladinFieldMap* magField = new R3BAladinFieldMap("AladinMaps");
        Double_t fMeasCurrent = 2500.; // I_current [A]
        magField->SetCurrent(fMeasCurrent);
        magField->SetScale(1.);
        run->SetField(magField);
        */

    /* Add analysis task ------------------------------------ */

    R3BPspxMapped2Precal* pspxMapped2Precal = new R3BPspxMapped2Precal("PspxMapped2Precal", 1);
    run->AddTask(pspxMapped2Precal);

    R3BPspxPrecal2Cal* pspxPrecal2Cal = new R3BPspxPrecal2Cal("PspxPrecal2Cal", 1);
    run->AddTask(pspxPrecal2Cal);

    R3BPspxCal2Hit* pspxCal2Hit = new R3BPspxCal2Hit("PspxCal2Hit", 1, 0.1);
    run->AddTask(pspxCal2Hit);

    /* ------------------------------------------------------ */

    R3BOnlineSpectra* r3bOnlineSpectra = new R3BOnlineSpectra("OnlineSpectra", 1);
    run->AddTask(r3bOnlineSpectra);

    /* Initialize ------------------------------------------- */
    run->Init();
    rtdb1->print();
      //  FairLogger::GetLogger()->SetLogScreenLevel("WARNING");
    //    FairLogger::GetLogger()->SetLogScreenLevel("DEBUG");
     FairLogger::GetLogger()->SetLogScreenLevel("ERROR");
    //FairLogger::GetLogger()->SetLogScreenLevel("INFO");

    /* Run -------------------------------------------------- */
    run->Run((nev < 0) ? nev : 0, (nev < 0) ? 0 : nev);
    //    rtdb1->saveOutput();

    timer.Stop();
    Double_t rtime = timer.RealTime();
    Double_t ctime = timer.CpuTime();
    cout << endl << endl;
    cout << "Macro finished succesfully." << endl;
    cout << "Output file is " << outputFileName << endl;
    cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << endl << endl;
}
