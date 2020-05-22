/*
 * Macro to run the online for califa and ams detectors simultaneously
 *
 * Author: Jose Luis <joseluis.rodriguez.sanchez@usc.es>
 * @since Feb. 9th , 2019
 * */

typedef struct EXT_STR_h101_t {
  EXT_STR_h101_unpack_t unpack;
  EXT_STR_h101_CALIFA_t califa;
  EXT_STR_h101_AMS_t ams;
} EXT_STR_h101;

void califa_ams_online() {
  TStopwatch timer;
  timer.Start();


  const Int_t nev = -1; /* number of events to read, -1 - until CTRL+C */


  /* Input --------------------------------------- */
  TString filename = "--stream=lxg0897:6002";
  //TString filename = "~/lmd/344_2019-02-16_17-51-36/data_00*.lmd";


  /* Output --------------------------------------- */
  TString outputFileName = "data_online.root";


  /* Calibration files ---------------------------- */
  TString dir = gSystem->Getenv("VMCWORKDIR");
  //CALIFA and AMS detectors
  TString califadir = dir + "/macros/r3b/unpack/califa_ams/";
  TString calfilename = califadir + "Califa_Ams_CalibParamFeb20190218.root";
  calfilename.ReplaceAll("//","/");


  /* Create source using ucesb for input ------------------ */
  //UCESB paths
  TString ntuple_options = "UNPACK:EVENTNO,UNPACK:TRIGGER,RAW";
  TString ucesb_dir = getenv("UCESB_DIR");
  TString ucesb_path = ucesb_dir + "/../upexps/201902_s444/201902_s444";
  ucesb_path.ReplaceAll("//","/");

  EXT_STR_h101 ucesb_struct;

  R3BUcesbSource* source = new R3BUcesbSource(filename, ntuple_options,
					      ucesb_path, &ucesb_struct, sizeof(ucesb_struct));
  source->SetMaxEvents(nev);


  /* Definition of reader --------------------------------- */
  R3BUnpackReader* unpackreader = new R3BUnpackReader((EXT_STR_h101_unpack*)&ucesb_struct,
					offsetof(EXT_STR_h101, unpack));

  R3BCalifaFebexReader* unpackcalifa = new R3BCalifaFebexReader((EXT_STR_h101_CALIFA*)&ucesb_struct.califa,
					     offsetof(EXT_STR_h101, califa));

  R3BAmsReader* unpackams= new R3BAmsReader((EXT_STR_h101_AMS*)&ucesb_struct.ams,
					     offsetof(EXT_STR_h101, ams));


  /* Add readers ------------------------------------------ */
  source->AddReader(unpackreader);
  unpackcalifa->SetOnline(true);
  source->AddReader(unpackcalifa);
  unpackams->SetOnline(true);
  source->AddReader(unpackams);


  /* Create online run ------------------------------------ */
  FairRunOnline* run = new FairRunOnline(source);
  run->SetRunId(1);
  run->SetSink(new FairRootFileSink(outputFileName));
  Int_t refresh = 2000;
  Int_t port=8046;
  run->ActivateHttpServer(refresh, port);


  /* Runtime data base ------------------------------------ */
  FairRuntimeDb* rtdb = run->GetRuntimeDb();


  /* Load parameters   ------------------------------------ */
  //CALIFA and AMS detectors
  FairParRootFileIo* parIo1 = new FairParRootFileIo();
  parIo1->open(calfilename,"in");
  rtdb->setFirstInput(parIo1);


  //R3BCalifaMapped2CrystalCal ---
  R3BCalifaMapped2CrystalCal* Map2CalCalifa = new R3BCalifaMapped2CrystalCal();
  Map2CalCalifa->SetOnline(true);
  run->AddTask(Map2CalCalifa);
  //R3BCalifaCrystalCal2Hit ---
  R3BCalifaCrystalCal2Hit* Cal2HitCalifa = new R3BCalifaCrystalCal2Hit();
  Cal2HitCalifa->SelectGeometryVersion(444);
  Cal2HitCalifa->SetSquareWindowAlg(0.25,0.25);  //[0.25 around 14.3 degrees, 3.2 for the complete calorimeter]
  Cal2HitCalifa->SetDetectionThreshold(200);     //200 KeV
  Cal2HitCalifa->SetDRThreshold(15000);          //15 MeV
  run->AddTask(Cal2HitCalifa);


  /* Add analysis tasks ----------------------------------- */
  R3BAmsMapped2StripCal* Map2CalAms = new R3BAmsMapped2StripCal();
  Map2CalAms->SetOnline(true);
  run->AddTask(Map2CalAms);
  R3BAmsStripCal2Hit* Cal2HitAms = new R3BAmsStripCal2Hit();
  Cal2HitAms->SetOnline(true);
  run->AddTask(Cal2HitAms);


  /* Add online tasks ------------------------------------- */
  //CALIFA detector
  Int_t petals=8;
  R3BCalifaOnlineSpectra* CalifaOnline= new R3BCalifaOnlineSpectra();
  CalifaOnline->SetPetals(petals);
  CalifaOnline->SetRange_max(100000);//100MeV
  run->AddTask(CalifaOnline);
  //AMS detectors
  R3BAmsOnlineSpectra* AmsOnline= new R3BAmsOnlineSpectra();
  run->AddTask(AmsOnline);
  //AMS and CALIFA correlations
  R3BAmsCalifaCorrelatedOnlineSpectra* AmsCalifaOnline= new R3BAmsCalifaCorrelatedOnlineSpectra();
  run->AddTask(AmsCalifaOnline);


  /* Initialize ------------------------------------------- */
  run->Init();
  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  //FairLogger::GetLogger()->SetLogScreenLevel("WARNING");
  //FairLogger::GetLogger()->SetLogScreenLevel("DEBUG");
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
  std::cout << "Real time " << rtime << " s, CPU time " << ctime << " s"
            << std::endl << std::endl;
  //gApplication->Terminate();
}
