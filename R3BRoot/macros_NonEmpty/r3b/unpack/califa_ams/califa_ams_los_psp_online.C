/*
 * Macro to run the online for califa and ams detectors simultaneously
 *
 * Author: Jose Luis <joseluis.rodriguez.sanchez@usc.es>
 * @since Feb. 9th , 2019
 * */

using namespace std;
typedef struct EXT_STR_h101_t {
  EXT_STR_h101_unpack_t unpack;
  EXT_STR_h101_CALIFA_t califa;
  EXT_STR_h101_LOS_TAMEX_onion los;
  EXT_STR_h101_AMS_t ams;
  EXT_STR_h101_PSP_onion_t psp;
  EXT_STR_h101_WRMASTER_t wrm;
  EXT_STR_h101_WRCALIFA_t wrcalifa;
  EXT_STR_h101_WRAMS_t wrams;
} EXT_STR_h101;

void califa_ams_los_psp_online() {
  TStopwatch timer;
  timer.Start();

  const Int_t nev = -1; /* number of events to read, -1 - until CTRL+C */

  const Int_t expId = 444; //select: 444 or 473
  Bool_t los_in  = true;
  Bool_t psp_in  = false;

  /* Input --------------------------------------- */
  TString filename = "--stream=lxir123.gsi.de:8006";
  //TString filename = "~/lmd/main0050_0621_sti.lmd";


  /* Output --------------------------------------- */
  TString outputFileName = "data_califa_ams_los_psp_online.root";


  /* Calibration files ---------------------------- */
  TString dir = gSystem->Getenv("VMCWORKDIR");
  //CALIFA and AMS detectors
  TString califadir = dir + "/macros/r3b/unpack/califa/";
  TString calfilename = califadir + "Califa_CalParam_20190306.root";
  calfilename.ReplaceAll("//","/");
  TString amsdir = dir + "/macros/r3b/unpack/ams/";
  TString amsfilename = amsdir + "Ams_CalibParam.root";
  amsfilename.ReplaceAll("//","/");
  TString losdir = dir + "/macros/r3b/unpack/parameters/";
  TString losfilename = losdir + "LosTcalPar_main054.root";
  losfilename.ReplaceAll("//","/");
  TString pspxpar_dir = dir + "/psp/par/";
  TString parPspxPrecalFileName= "s444_pspx_precal_default_6det.par";
  TString parPspxCalFileName= "s444_pspx_cal_default_6det.par";;
  TString parPspxHitFileName="s444_pspx_hit_default_6det.par";

  /* Create source using ucesb for input ------------------ */
  //UCESB paths
  TString ntuple_options = "RAW";
  TString upexps_dir = dir+"/../upexps/";
  TString ucesb_path;
  if(expId==444){
    ucesb_path = upexps_dir+"/201902_s444/201902_s444 --allow-errors --input-buffer=100Mi";
  }else if(expId==473){
    ucesb_path = upexps_dir+"/201902_s473/201902_s473 --allow-errors --input-buffer=100Mi";
  }else{
    std::cout<<"Experiment was not selected!"<<std::endl;
    gApplication->Terminate();
  }
  ucesb_path.ReplaceAll("//","/");


  /* Definition of Ucesb --------------------------------- */

  EXT_STR_h101 ucesb_struct;
  R3BUcesbSource* source = new R3BUcesbSource(filename, ntuple_options,
					      ucesb_path, &ucesb_struct, sizeof(ucesb_struct));
  source->SetMaxEvents(nev);

  /* Definition of reader --------------------------------- */
  R3BUnpackReader* unpackreader = new R3BUnpackReader((EXT_STR_h101_unpack*)&ucesb_struct,
					offsetof(EXT_STR_h101, unpack));

  R3BCalifaFebexReader* unpackcalifa = new R3BCalifaFebexReader((EXT_STR_h101_CALIFA*)&ucesb_struct.califa,
					     offsetof(EXT_STR_h101, califa));

  R3BAmsReader* unpackams = new R3BAmsReader((EXT_STR_h101_AMS*)&ucesb_struct.ams,
					     offsetof(EXT_STR_h101, ams));


  /* Definition of reader --------------------------------- */
  R3BUnpackReader* unpackreader = new R3BUnpackReader((EXT_STR_h101_unpack*)&ucesb_struct,
					     offsetof(EXT_STR_h101, unpack));
  R3BCalifaFebexReader* unpackcalifa = new R3BCalifaFebexReader((EXT_STR_h101_CALIFA*)&ucesb_struct.califa,
					     offsetof(EXT_STR_h101, califa));
  R3BAmsReader* unpackams = new R3BAmsReader((EXT_STR_h101_AMS*)&ucesb_struct.ams,
					     offsetof(EXT_STR_h101, ams));
  R3BWhiterabbitMasterReader* unpackWRM = new R3BWhiterabbitMasterReader((EXT_STR_h101_WRMASTER*)&ucesb_struct.wrm,
                                             offsetof(EXT_STR_h101, wrm), 0x100);
  R3BWhiterabbitCalifaReader* unpackWRC = new R3BWhiterabbitCalifaReader((EXT_STR_h101_WRCALIFA*)&ucesb_struct.wrcalifa,
                                             offsetof(EXT_STR_h101, wrcalifa), 0x400);
  R3BWhiterabbitAmsReader* unpackWRA = new R3BWhiterabbitAmsReader((EXT_STR_h101_WRAMS*)&ucesb_struct.wrams,
                                             offsetof(EXT_STR_h101, wrams), 0x300);


  /* Add readers ------------------------------------------ */
  source->AddReader(unpackreader);
  source->AddReader(unpackWRM);
  source->AddReader(unpackWRC);
  source->AddReader(unpackWRA);
  unpackcalifa->SetOnline(true);
  source->AddReader(unpackcalifa);
  unpackams->SetOnline(true);
  source->AddReader(unpackams);

  if(los_in){
   R3BLosReader *unpacklos = new R3BLosReader((EXT_STR_h101_LOS_TAMEX *)&ucesb_struct.los,
	                          offsetof(EXT_STR_h101, los));
   unpacklos->SetOnline(true);
   source->AddReader(unpacklos);
  }

  if(psp_in){
   R3BPspxReader* unpackpsp= new R3BPspxReader((EXT_STR_h101_PSP*)&ucesb_struct.psp,
                                  offsetof(EXT_STR_h101, psp));
   unpackpsp->SetOnline(true);
   source->AddReader(unpackpsp);
  }

  Int_t RunId=3;
  /* Create online run ------------------------------------ */
  FairRunOnline* run = new FairRunOnline(source);
  run->SetRunId(RunId);
  run->SetSink(new FairRootFileSink(outputFileName));
  Int_t refresh = 1000;
  Int_t port=8046;
  run->ActivateHttpServer(refresh, port);


  /* Runtime data base ------------------------------------ */
  FairRuntimeDb* rtdb = run->GetRuntimeDb();


  /* Load parameters   ------------------------------------ */
  //CALIFA and AMS detectors
  Bool_t kParameterMerged = kFALSE;
  FairParRootFileIo* parIo1 = new FairParRootFileIo(kParameterMerged);
  TList *parList = new TList();
  parList->Add(new TObjString(calfilename));
  parList->Add(new TObjString(amsfilename));
  //Add LOS detector
  if(los_in) parList->Add(new TObjString(losfilename));
  parIo1->open(parList);
  rtdb->setFirstInput(parIo1);
  rtdb->print();


  FairParAsciiFileIo* parIo2 = new FairParAsciiFileIo();
  TList* parList2 = new TList();
  //Add PSPX detectors
  if(psp_in){
  parList2->Add(new TObjString(pspxpar_dir + parPspxPrecalFileName));
  parList2->Add(new TObjString(pspxpar_dir + parPspxCalFileName));
  parList2->Add(new TObjString(pspxpar_dir + parPspxHitFileName));
  parIo2->open(parList2);
  rtdb->setSecondInput(parIo2);
  }

  // LOS
  if(los_in){
    rtdb->addRun(RunId);
    rtdb->getContainer("LosTCalPar");
    rtdb->setInputVersion(RunId, (char*)"LosTCalPar", 1, 1);
  }

  //Califa and ams detectors
  rtdb->getContainer("califaCrystalCalPar");
  rtdb->setInputVersion(RunId, (char*)"califaCrystalCalPar", 1, 1);
  rtdb->getContainer("amsStripCalPar");
  rtdb->setInputVersion(RunId, (char*)"amsStripCalPar", 1, 1);
  if(psp_in){
  //rtdb->addRun(RunId);
  rtdb->getContainer("R3BPspxPrecalPar");
  rtdb->setInputVersion(RunId, (char*)"R3BPspxPrecalPar", 1, 1);
  //rtdb->addRun(1);
  rtdb->getContainer("R3BPspxCalPar");
  rtdb->setInputVersion(RunId, (char*)"R3BPspxCalPar", 1, 1);
  //rtdb->addRun(1);
  rtdb->getContainer("R3BPspxHitPar");
  rtdb->setInputVersion(RunId, (char*)"R3BPspxHitPar", 1, 1);
  }


  /* Add analysis tasks ----------------------------------- */

  //R3BCalifaMapped2CrystalCal ---
  R3BCalifaMapped2CrystalCal* Map2CalCalifa = new R3BCalifaMapped2CrystalCal();
  Map2CalCalifa->SetOnline(true);
  run->AddTask(Map2CalCalifa);
  //R3BCalifaCrystalCal2Hit ---
  R3BCalifaCrystalCal2Hit* Cal2HitCalifa = new R3BCalifaCrystalCal2Hit();
  Cal2HitCalifa->SelectGeometryVersion(444);
  Cal2HitCalifa->SetSquareWindowAlg(0.25,0.25);   //[0.25 around 14.3 degrees, 3.2 for the complete calorimeter]
  Cal2HitCalifa->SetDetectionThreshold(200);    //200 KeV
  Cal2HitCalifa->SetDRThreshold(500);           //0.5 MeV
  run->AddTask(Cal2HitCalifa);

  R3BAmsMapped2StripCal* Map2CalAms = new R3BAmsMapped2StripCal();
  Map2CalAms->SetOnline(true);
  Map2CalAms->SetThresholdSigma(5);
  run->AddTask(Map2CalAms);
  R3BAmsStripCal2Hit* Cal2HitAms = new R3BAmsStripCal2Hit();
  Cal2HitAms->SetOnline(true);
  Cal2HitAms->SetMaxNumClusters(20);
  run->AddTask(Cal2HitAms);

  if(psp_in){
   R3BPspxMapped2Precal* pspxMapped2Precal = new R3BPspxMapped2Precal();
   run->AddTask(pspxMapped2Precal);
   R3BPspxPrecal2Cal* pspxPrecal2Cal = new R3BPspxPrecal2Cal();
   run->AddTask(pspxPrecal2Cal);
   R3BPspxCal2Hit* pspxCal2Hit = new R3BPspxCal2Hit();
   run->AddTask(pspxCal2Hit);
  }

  if(los_in){
   R3BLosMapped2Cal* losMapped2Cal=new R3BLosMapped2Cal("LosTCalPar", 1);
   losMapped2Cal->SetNofModules(1,8);
   losMapped2Cal->SetTrigger(1);//FIXME
   run->AddTask( losMapped2Cal );

   R3BLosCal2Hit* losCal2Hit=new R3BLosCal2Hit("losCal2Hit", 1);

// Set parameters for X,Y calibration
// offsetX1, offsetY1,VeffX1,VeffY1, offsetX2, offsetY2,VeffX2,VeffY2
   losCal2Hit->SetLosParamMCFD(0., 0., 1., 1. ,0., 0., 1., 1.);                 // spectra X1_vs_Y1, X2_vs_Y2
   losCal2Hit->SetLosParamMCFDwc(0., 0., 1., 1. ,0., 0., 1., 1.);               // spectra X1_vs_Y1_wc, X2_vs_Y2_wc
   losCal2Hit->SetLosParamToT(0., 0., 1., 1. ,0., 0., 1., 1.);
   losCal2Hit->SetLosParamTAMEX(0., 0., 1., 1. ,0., 0., 1., 1.);                // spectra XT1_vs_YT1, XT2_vs_YT2
   losCal2Hit->SetLosParamToTc(0., 0., 1., 1. ,0., 0., 1., 1.);

// Set Sci shape, variable iSciType:
// 0 - Large (10x10) squared, with 2-inch PM
// 1 - Small octagonal, with 1-inch PM
// 2 - Small (5x5) squared, with 2-inch PM
// 3 - Large octagonal, with 2-inch PM
// 4 - Large (10x10) squared, with 1-inch PM
// 5 - Large octagional, with 4 2-inch and 4 1inch PMs
// 6 - Large octagional, with 8 1-inch PMs

// losCal2Hit->SetLosInput(iSciType,walk_param_file_name,totcorrection_factor_file_name);

   losCal2Hit->SetLosInput(6,"walk_param_empty.dat","tot_param_empty.dat");
   run->AddTask( losCal2Hit );
  }


  /* Add online tasks ------------------------------------- */
  //CALIFA detector
  Int_t petals=8;
  R3BCalifaOnlineSpectra* CalifaOnline= new R3BCalifaOnlineSpectra();
  CalifaOnline->SetPetals(petals);
  CalifaOnline->SetRange_max(50000);//50MeV
  run->AddTask(CalifaOnline);
  //AMS detectors
  R3BAmsOnlineSpectra* AmsOnline= new R3BAmsOnlineSpectra();
  run->AddTask(AmsOnline);
  //AMS and CALIFA correlations
  R3BAmsCalifaCorrelatedOnlineSpectra* AmsCalifaOnline= new R3BAmsCalifaCorrelatedOnlineSpectra();
  run->AddTask(AmsCalifaOnline);

  //R3BOnlineSpectra* r3bOnlineSpectra=new R3BOnlineSpectra("OnlineSpectra", 1);
  //r3bOnlineSpectra->SetTrigger(trigger_value);
  //run->AddTask( r3bOnlineSpectra );


  /* Initialize ------------------------------------------- */
  run->Init();
  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  //FairLogger::GetLogger()->SetLogScreenLevel("WARNING");
  //FairLogger::GetLogger()->SetLogScreenLevel("DEBUG");
  /* ------------------------------------------------------ */


  /* Run -------------------------------------------------- */
  run->Run((nev < 0) ? nev : 0, (nev < 0) ? 0 : nev);


  // -----   Finish   -------------------------------------------------------
  cout << endl << endl;
  // Extract the maximal used memory an add is as Dart measurement
  // This line is filtered by CTest and the value send to CDash
  FairSystemInfo sysInfo;
  Float_t maxMemory=sysInfo.GetMaxMemory();
  cout << "MaxMemory: ";
  cout << maxMemory<< endl;

  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  Float_t cpuUsage=ctime/rtime;
  cout << "CPU used: " << cpuUsage << endl;

  cout << endl;
  std::cout << "Output file is " << outputFileName << std::endl;
  cout << "Real time " << rtime << " s, CPU time " << ctime << "s"
       << endl << endl;
  cout << "Macro finished successfully." << endl;
  //gApplication->Terminate();
}
