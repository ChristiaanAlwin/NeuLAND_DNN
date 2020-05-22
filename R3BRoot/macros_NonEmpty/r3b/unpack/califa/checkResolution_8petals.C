////-------------------------------------------------------------------------
//// ** joseluis.rodriguez.sanchez@usc.es
////    Universidad de Santiago de Compostela & GSI
////-------------------------------------------------------------------------
////
////	--- Check Resutls of CALIFA detector ---
////
////	Macro to check:
////	   -Resolution of one peak, set to the Co60 source, peak of 1.33MeV
////
////	General Usage:
////	1st: select the root file and the settings:
////	   -maximum energy & channels, range CryId of the petals 
////	   -Histograms: change the ranges and bins (OPTIONAL)
////
////	2nd: root -l checkResolution_8petals.C
////
////--------------------------------------------------------------------------


void checkResolution_8petals() {
  //ROOT ENVIRONMENT
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(1);
  gStyle->SetOptFit(0);
  
  ////
  //// SETTINGS: SELECT HERE THE SETTING CONTROLING THE MACRO
  ////
  //INPUT FILE
  char inputFile[250] = "dataCal_60Co_27Jul.root";  //change root file
  TFile *file1 = TFile::Open(inputFile);
  //READING TREE
  TTree* tree = (TTree*)file1->Get("evt");
  
  //VARIABLES DEFINITION
  
  //Energy range for histograms in KeV
  Double_t maxE;
  Double_t minE;
  Int_t binEnergy;
  minE=800;
  maxE=1600;
  binEnergy=400;

  //Fit range in KeV
  Double_t maxEfit=1370.0;
  Double_t minEfit=1280.0;
  
 
  //HISTOGRAMS DEFINITION-----------------------------------------------------------
 
    //HISTOGRAMS   ------------------------------------------------------------
  Int_t nbcry=513;//64*8+1
  TH1F * hcrystal[nbcry];
  char buf[126], buf2[126];
  for(Int_t i=0;i<nbcry;i++){
  sprintf(buf,"hcrystal_%i",i+1);
  sprintf(buf2,"Crystal %i",i+1);
  hcrystal[i] = new TH1F(buf,buf2,binEnergy,minE,maxE);
  }
  
  
  //----- CALIFA detector--------------//
  
  //Mapped Data
  TClonesArray* mappedDataCA;
  R3BCalifaMappedData** mappedData;
  mappedDataCA = new TClonesArray("R3BCalifaMappedData",5);
  TBranch *branchCalifaMappedData = tree->GetBranch("CalifaMappedData");
  branchCalifaMappedData->SetAddress(&mappedDataCA);
  
  //Calibrated Data
  TClonesArray* calDataCA;
  R3BCalifaCrystalCalData** calData;
  calDataCA = new TClonesArray("R3BCalifaCrystalCalData",5);
  TBranch *branchCalifaCalData = tree->GetBranch("CalifaCrystalCalData");
  branchCalifaCalData->SetAddress(&calDataCA);
  
  Int_t mapHitsPerEvent = 0;
  Int_t caloHitsPerEvent= 0;
  
  //TREE ENTRIES--------------------------------------------------------------------
  Long64_t nevents = tree->GetEntries();
  
  //LOOP IN THE EVENTS--------------------------------------------------------------
  for(int i=0;i<nevents;i++){
    if(i%1== 100) printf("Event:%i\n",i);
    tree->GetEvent(i);
    
    // mapHitsPerEvent = mappedDataCA->GetEntries();
    caloHitsPerEvent= calDataCA->GetEntries();
    /*   
     if(mapHitsPerEvent>0) {
       mappedData = new R3BCalifaMappedData*[mapHitsPerEvent];
       for(Int_t j=0;j<mapHitsPerEvent;j++){
	 mappedData[j] = new R3BCalifaMappedData;
	 mappedData[j] = (R3BCalifaMappedData*) mappedDataCA->At(j);
       }
    }
   */  
     if(caloHitsPerEvent>0) {
       calData = new R3BCalifaCrystalCalData*[caloHitsPerEvent];
       for(Int_t j=0;j<caloHitsPerEvent;j++){
	 calData[j] = new R3BCalifaCrystalCalData;
	 calData[j] = (R3BCalifaCrystalCalData*) calDataCA->At(j);
      }
     }
     
     //ANALYSIS of the Results ------------------------
    
      //LOOP in Cal Hits ----
     for(Int_t h=0;h<caloHitsPerEvent;h++){
           
       hcrystal[calData[h]->GetCrystalId()]->Fill(calData[h]->GetEnergy());
       
     }
     
     //
     if(caloHitsPerEvent)     delete[] calData;
  }
  // END LOOP IN THE EVENTS----------------------------------------------------------------------------------------------------

  TF1 *f1 = new TF1 ("f1", "gaus", minEfit, maxEfit);

  TCanvas* c0 = new TCanvas("Crystalvsresol","CrystalId",0,0,800,400);

  int v=0;

  double x[nbcry],y[nbcry];
  
  for(int i=0;i<nbcry;i++){
    x[i]=0.;y[i]=0.;
    if(hcrystal[i]->Integral()>0){
    hcrystal[i]->Fit("f1","QR");
    std::cout << i-1 <<"\t"<< 235*f1->GetParameter(2)/f1->GetParameter(1)<< "% \t" << f1->GetChisquare()  << std::endl;
    x[i]=i-1;
    y[i]=235*f1->GetParameter(2)/f1->GetParameter(1);
    v++;
   }
  }
  TGraph *g = new TGraph(nbcry,x,y);
  g->SetTitle("Peak energy = 1.33MeV; Number of crystal; resolution (FWHM=2.35*100*sigma/mean)");
  g->SetMarkerStyle(21);
  g->SetMarkerColor(2);
  g->Draw("ap");

TCanvas* c1 = new TCanvas("Crys","CrystalId",0,0,800,400);

 hcrystal[189]->Draw("");
 
  
 
}
