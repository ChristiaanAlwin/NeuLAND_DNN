/* Additional info:
 * To generate the header file used for the R3BUcesbSource (ext_h101.h), use:
 *
 * ./201902_s444 --ntuple=RAW:CALIFA,id=h101_CALIFA,ext_h101_raw_califa_febex.h
 *
 * at $UCESB_DIR/upexps/201902_s444
 *
 * Put the header file into the 'r3bsource' directory and recompile.
 *
 * @since October 23th, 2018
 * */

typedef struct EXT_STR_h101_t {
  EXT_STR_h101_unpack_t unpack;
  EXT_STR_h101_CALIFA_t califa;
  EXT_STR_h101_WRMASTER_t wrm;
  EXT_STR_h101_WRCALIFA_t wrcalifa;
} EXT_STR_h101;


void califa_online(){
  TStopwatch timer;
  timer.Start();

  const Int_t nev = -1;     // number of events to read, -1 - until CTRL+C

  const Int_t expId = 473; //select: 444, 454 or 473

  // Create source using ucesb for input ---------------------------------------
  TString filename = "--stream=lxir123.gsi.de:8006";
  //TString filename = "~/lmd/main0223_st.lmd";


  /* Output --------------------------------------- */
  TString outputFileName = "./data_califa_online.root";


  /* Calibration files ---------------------------- */
  TString dir = gSystem->Getenv("VMCWORKDIR");
  //CALIFA detector
  TString califadir = dir + "/macros/r3b/unpack/califa/";
  TString calfilename = califadir + "Califa_CalParam_20190306.root";
  calfilename.ReplaceAll("//","/");


  /* Create source using ucesb for input ------------------ */
  //UCESB paths
  TString ntuple_options = "RAW";
  TString upexps_dir = dir+"/../upexps/";
  TString ucesb_path;
  if(expId==444){
    ucesb_path = upexps_dir+"/201902_s444/201902_s444 --allow-errors --input-buffer=100Mi";
  }else if(expId==454){
    ucesb_path = upexps_dir+"/201904_s454/201904_s454 --allow-errors --input-buffer=100Mi";
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
  R3BUnpackReader* unpackreader = new R3BUnpackReader((EXT_STR_h101_unpack*)&ucesb_struct.unpack,
					     offsetof(EXT_STR_h101, unpack));
  R3BCalifaFebexReader *unpackcalifa = new R3BCalifaFebexReader((EXT_STR_h101_CALIFA*)&ucesb_struct.califa,
					     offsetof(EXT_STR_h101, califa));
  R3BWhiterabbitMasterReader* unpackWRM = new R3BWhiterabbitMasterReader((EXT_STR_h101_WRMASTER*)&ucesb_struct.wrm,
                                             offsetof(EXT_STR_h101, wrm), 0x100);
  R3BWhiterabbitCalifaReader* unpackWRC = new R3BWhiterabbitCalifaReader((EXT_STR_h101_WRCALIFA*)&ucesb_struct.wrcalifa,
                                             offsetof(EXT_STR_h101, wrcalifa), 0x400);


  /* Add readers ------------------------------------------ */
  source->AddReader(unpackreader);
  unpackWRM->SetOnline(true);
  source->AddReader(unpackWRM);
  unpackWRC->SetOnline(true);
  source->AddReader(unpackWRC);
  unpackcalifa->SetOnline(true);
  source->AddReader(unpackcalifa);


  // Create online run ---------------------------------------------------------
  FairRunOnline* run = new FairRunOnline(source);
  run->SetRunId(1);
  run->SetSink(new FairRootFileSink(outputFileName));
  Int_t refresh = 2000;
  Int_t port=8089;
  run->ActivateHttpServer(refresh,port);


  // Runtime data base ---------------------------------------------------------
  FairRuntimeDb* rtdb = run->GetRuntimeDb();


  /* Load parameters   ------------------------------------ */
  //Choose Root or Ascii file
  //1-Root file with the Calibartion Parameters
  FairParRootFileIo* parIo1 = new FairParRootFileIo();
  parIo1->open(calfilename,"in");
  rtdb->setFirstInput(parIo1);
  //2-Ascii file with the Calibartion Parameters
  /*FairParAsciiFileIo* parIo1 = new FairParAsciiFileIo();
    parIo1->open(calfilename,"out");
    rtdb->setOutput(parIo1);
    rtdb->saveOutput();
  */


  // Create analysis task ------------------------------------------------------
  //R3BCalifaMapped2CrystalCal ---
  R3BCalifaMapped2CrystalCal* Map2Cal = new R3BCalifaMapped2CrystalCal();
  Map2Cal->SetOnline(true);
  run->AddTask(Map2Cal);
  //R3BCalifaCrystalCal2Hit ---
  R3BCalifaCrystalCal2Hit* Cal2Hit = new R3BCalifaCrystalCal2Hit();
  Cal2Hit->SetOnline(true);
  Cal2Hit->SelectGeometryVersion(444);    //444 version is for S444, S454 and S473 experiments
  Cal2Hit->SetSquareWindowAlg(0.25,0.25);     //[0.25 around 14.3 degrees, 3.2 for the complete calorimeter]
  Cal2Hit->SetDetectionThreshold(200);    //200 KeV
  Cal2Hit->SetDRThreshold(2000);          //2 MeV
  run->AddTask(Cal2Hit);


  // R3BOnlineSpectra ----------------------------------------------------------
  Int_t petals=8; //8th petal is for Dual Preamps
  //
  R3BCalifaOnlineSpectra* CalifaOnline= new R3BCalifaOnlineSpectra();
  CalifaOnline->SetPetals(petals);
  CalifaOnline->SetRange_max(10000);//10MeV
  run->AddTask(CalifaOnline);


  // Initialize ----------------------------------------------------------------
  run->Init();
  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  //    FairLogger::GetLogger()->SetLogScreenLevel("WARNING");
  //    FairLogger::GetLogger()->SetLogScreenLevel("DEBUG");


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
