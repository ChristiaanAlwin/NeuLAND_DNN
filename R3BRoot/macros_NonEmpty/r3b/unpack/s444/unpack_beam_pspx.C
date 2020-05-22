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

/*
extern "C" {
//#include "/home/bloeher/git/R3BRoot/r3bsource/ext_h101_full.h"
//#include "ext_data_client.h"
#include "/home/land/R3BRoot_PSPX/r3bsource/ext_h101_psp.h" 
#include "/home/land/R3BRoot_PSPX/r3bsource/ext_h101_los.h" 
}
*/

typedef struct EXT_STR_h101_t
{
  //EXT_STR_h101_LOS_onion_t los;
  EXT_STR_h101_unpack_t unpack;
  EXT_STR_h101_PSP_onion_t psp;
} EXT_STR_h101;

void unpack_beam_pspx(Int_t RunId=12)
{
    TString runNumber=Form("%04d", RunId);
    TStopwatch timer;
    timer.Start();

    const Int_t nev = -1; /* number of events to read, -1 - until CTRL+C */
    //const Int_t nev = 100;

    /* Create source using ucesb for input ------------------ */
     
    TString filename = "/home/land/PSPX/lmd_s473/main"+runNumber+"_*.lmd"; //_00[0-9][0-9]
    //TString filename = "/home/land/PSPX/lmd_beamtest/pspx1_pspx2_run"+runNumber+"_0040.lmd"; //_00[0-9][0-9]
  
    //TString outputFileName = "/home/land/PSPX/rootfiles_tests/s473_tests/pspx_run"+runNumber+"_mapped.root";
    TString outputFileName = "/home/land/PSPX/rootfiles_tests/s473_tests/pspx_run"+runNumber+"_cal.root";

    TString ntuple_options = "UNPACK:EVENTNO,UNPACK:TRIGGER,RAW,PSPX";
    //TString ucesb_dir = getenv("UCESB_DIR");
    //TString ucesb_path = ucesb_dir + "/../upexps/jun16Xe/jun16Xe";
    //TString ucesb_path = "/home/land/upexps/kvi2018/kvi2018";
    TString ucesb_path = "/home/land/upexps/201902_s473/201902_s473 --input-buffer=100Mi";
    //TString ucesb_path = "/home/land/upexps/alpha18/kvi2018";

    TString pspxpar_dir = "/home/land/R3BRoot_PSPX/psp/par/";
    TString parPspxMappedFileName = "s444_pspx_mapped_6det.par";
    //TString parPspxPrecalFileName = "s444_pspx_precal_190224_6det.par";
    TString parPspxPrecalFileName = "s444_pspx_precal_default_6det.par";
    //TString parPspxPrecalFileName = "s444_pspx_precal_run197_pos1_en23_6det.par";
    //TString parPspxCalFileName = "s444_pspx_cal_default_6det.par";
    TString parPspxCalFileName = "s444_pspx_cal_run197_en123_6det.par";
    TString parPspxHitFileName = "s444_pspx_hit_default_6det.par";
    
    //TString parPspxPrecalFileName = "kvi2018_pspx_precal_default.par";
    //TString parPspxMappedFileName = "kvi2018_pspx_mapped.par";
    //TString parPspxCalFileName = "kvi2018_pspx_cal_default.par";
    //TString parPspxHitFileName = "kvi2018_pspx_hit_default.par";
    
    //TString parPspxPrecalFileName = "alpha18_det1_shiftedalpha_pspx_precal.par";
    //TString parPspxCalFileName = "alpha18_det1_shiftedalpha_pspx_cal.par";
    //TString parPspxCalFileName = "alpha18_det2_pspx_cal_filter120.par";
    
    EXT_STR_h101 ucesb_struct;
    R3BUcesbSource* source = new R3BUcesbSource(filename, ntuple_options,
	ucesb_path, &ucesb_struct, sizeof(ucesb_struct));
    source->SetMaxEvents(nev);

    source->AddReader( new R3BUnpackReader ((EXT_STR_h101_unpack*)&ucesb_struct.unpack, offsetof(EXT_STR_h101, unpack)) );
  //  source->AddReader( new R3BLosReader ((EXT_STR_h101_LOS*)&ucesb_struct.los, offsetof(EXT_STR_h101, los)) );
    source->AddReader( new R3BPspxReader((EXT_STR_h101_PSP*)&ucesb_struct.psp, offsetof(EXT_STR_h101, psp)) );
   
    
   /* Create online run ------------------------------------ */
    //const Int_t refresh = 100;
    FairRunOnline* run = new FairRunOnline(source);
    run->SetRunId(01);

    // Create analysis run ----------------------------------
    //FairRunAna* run = new FairRunAna();
    //run->SetSource(new FairFileSource(filename)); //necessary for rootfile
    //run->SetInputFile(filename);
    
    run->SetOutputFile(outputFileName.Data());


    /* Runtime data base ------------------------------------ */
    FairRuntimeDb* rtdb1 = run->GetRuntimeDb();

    FairParAsciiFileIo* parInput = new  FairParAsciiFileIo();
    TList *parList = new TList();
    parList->Add(new TObjString(pspxpar_dir+parPspxMappedFileName));
    parList->Add(new TObjString(pspxpar_dir+parPspxPrecalFileName));
    parList->Add(new TObjString(pspxpar_dir+parPspxCalFileName));
    parList->Add(new TObjString(pspxpar_dir+parPspxHitFileName));
    parInput->open(parList);
    rtdb1->setFirstInput(parInput);
    rtdb1->print();

    /* Add analysis task ------------------------------------ */


    R3BPspxMapped2Precal* pspxMapped2Precal = new R3BPspxMapped2Precal("PspxMapped2Precal", 1);
    run->AddTask(pspxMapped2Precal);
    R3BPspxPrecal2Cal* pspxPrecal2Cal = new R3BPspxPrecal2Cal("PspxPrecal2Cal", 1);
    run->AddTask(pspxPrecal2Cal);
    R3BPspxCal2Hit* pspxCal2Hit = new R3BPspxCal2Hit("PspxCal2Hit", 1, 20);
    run->AddTask(pspxCal2Hit);
 /* */
    /* Initialize ------------------------------------------- */
    ((R3BPspxPrecalPar*)FairRuntimeDb::instance()->getContainer("R3BPspxPrecalPar"))->printparams();
    run->Init();
    rtdb1->print();
    //    FairLogger::GetLogger()->SetLogScreenLevel("WARNING");
    //    FairLogger::GetLogger()->SetLogScreenLevel("DEBUG");
    FairLogger::GetLogger()->SetLogScreenLevel("INFO");

    /* Run -------------------------------------------------- */
    run->Run((nev < 0) ? nev : 0, (nev < 0) ? 0 : nev);
//    rtdb1->saveOutput();

    timer.Stop();
    Double_t rtime = timer.RealTime();
    Double_t ctime = timer.CpuTime();
    cout << endl << endl;
    cout << "Macro finished succesfully." << endl;
    cout << "Output file is " << outputFileName << endl;
    cout << "Real time " << rtime << " s, CPU time " << ctime << " s"
         << endl << endl;

    gApplication->Terminate();
}
