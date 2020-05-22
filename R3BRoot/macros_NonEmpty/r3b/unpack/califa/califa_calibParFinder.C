/* Additional info:
 * To generate the header file used for the R3BUcesbSource (ext_h101.h), use:
 *
 * ./201902_s444 --ntuple=RAW:CALIFA,id=h101_CALIFA,ext_h101_raw_califa_febex.h
 *
 * at $UCESB_DIR/upexps/201902_s444
 *
 * Put the header file into the 'r3bsource' directory and recompile.
 *
 */

typedef struct EXT_STR_h101_t {
  EXT_STR_h101_unpack_t unpack;
  EXT_STR_h101_CALIFA_t califa;
} EXT_STR_h101;

void califa_calibParFinder() {
  TStopwatch timer;
  timer.Start();
  
  const Int_t nev = -1; /* number of events to read, -1 - until CTRL+C */
  
  /* Create source using ucesb for input ------------------ */
  TString filename = "--stream=lxg0898:6002";
  //TString filename = "~/lmd/run_1550262804.lmd";
  TString outputFileName = "./datacappar_0001.root";


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
  
  source->AddReader(new R3BUnpackReader((EXT_STR_h101_unpack*)&ucesb_struct,
					offsetof(EXT_STR_h101, unpack)));
  source->AddReader(new R3BCalifaFebexReader((EXT_STR_h101_CALIFA*)&ucesb_struct.califa,
					     offsetof(EXT_STR_h101, califa)));

  
  /* Create online run ------------------------------------ */
  FairRunOnline* run = new FairRunOnline(source);
  run->SetRunId(1);
  run->SetSink(new FairRootFileSink(outputFileName));
  
  /* Add analysis task ------------------------------------ */
  
  //R3BCalifaMapped2CrystalCalPar ----
  TArrayF* EnergythePeaks = new TArrayF();
  Float_t e1=1332.5;
  Float_t e2=1173.2;
  EnergythePeaks->Set(2);
  EnergythePeaks->AddAt(e1,0);
  EnergythePeaks->AddAt(e2,1);
  
  R3BCalifaMapped2CrystalCalPar* CalPar = new R3BCalifaMapped2CrystalCalPar();  
  CalPar->SetNumCrystals(6912);//Max. crystal ID
  CalPar->SetMinStadistics(1000);
  CalPar->SetNumParameterFit(2);//OPTIONAL by default 2
  //Gamma range
  CalPar->SetCalRange_left(700);  
  CalPar->SetCalRange_right(1300);
  CalPar->SetCalRange_bins(100);
  //particle range  
  CalPar->SetCalRangeP_left(60);  
  CalPar->SetCalRangeP_right(130);
  CalPar->SetCalRangeP_bins(50);   
  CalPar->SetSigma(3.0);
  CalPar->SetThreshold(0.0001);      
  CalPar->SetEnergyPeaks(EnergythePeaks);
  run->AddTask(CalPar);

  
  /* Initialize ------------------------------------------- */
  run->Init();
  //    FairLogger::GetLogger()->SetLogScreenLevel("WARNING");
  //    FairLogger::GetLogger()->SetLogScreenLevel("DEBUG");
  FairLogger::GetLogger()->SetLogScreenLevel("INFO");

  
  /* Runtime data base ------------------------------------ */
  FairRuntimeDb* rtdb = run->GetRuntimeDb();

  
  //Choose Root or Ascii file	
  //1-Root file with the Calibartion Parameters
  Bool_t kParameterMerged = kTRUE;
  FairParRootFileIo* parOut = new FairParRootFileIo(kParameterMerged);
  parOut->open("Califa_CalibParamFeb2019.root");
  rtdb->setOutput(parOut);
  
  //2-Ascii file with the Calibartion Parameters
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
  cout << "Real time " << rtime << " s, CPU time " << ctime << " s"
       << endl << endl;
}
