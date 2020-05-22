/* 
 * Macro to check the timestamps for the general DAQ, califa and ams detectors
 *
 * Author: Jose Luis <joseluis.rodriguez.sanchez@usc.es>
 * @since March. 12th , 2019
 * */

void ana_wr(){
    //DEBUG  (optional)   -----------------------------------------------------
    gDebug = 0;

    //STYLE   -----------------------------------------------------------------		
    //gROOT->SetStyle("Default");
    gStyle->SetOptStat(1);
    gStyle->SetOptFit(0);

    //DEFINE THE INPUT FILE  --------------------------------------------------
    TString filename = "data_online.root";

    //OPEN ROOT FILE  ---------------------------------------------------------
    TFile *file1 = TFile::Open(filename);

    //OPEN TREE    ------------------------------------------------------------
    TTree* TWrs = (TTree*)file1->Get("evt");

    //WR master
    TClonesArray* WRMasterCA;  
    R3BWRMasterData** wrmasterData;
    WRMasterCA = new TClonesArray("R3BWRMasterData",5);
    TBranch *branchwrmHit = TWrs->GetBranch("WRMasterData");
    branchwrmHit->SetAddress(&WRMasterCA);

    //WR califa
    TClonesArray* WRCalifaCA;  
    R3BWRCalifaData** wrcalifaData;
    WRCalifaCA = new TClonesArray("R3BWRCalifaData",5);
    TBranch *branchwrcHit = TWrs->GetBranch("WRCalifaData");
    branchwrcHit->SetAddress(&WRCalifaCA);

    //WR ams
    TClonesArray* WRAmsCA;  
    R3BWRAmsData** wramsData;
    WRAmsCA = new TClonesArray("R3BWRAmsData",5);
    TBranch *branchwraHit = TWrs->GetBranch("WRAmsData");
    branchwraHit->SetAddress(&WRAmsCA);

    TH1F* hwrmul = new TH1F("hwr_hits","Nb Hits",10000,-7.,7.);
    TH1D* hwrm = new TH1D("hwrm","WR MASTER",12000,0,500);
    hwrm->GetYaxis()->SetTitle("Counts");
    hwrm->GetYaxis()->CenterTitle(true);
    hwrm->GetYaxis()->SetTitleOffset(1.2);
    TH1D* hwca = new TH1D("hwca","WRs: CALIFA - AMS",300,0.,4500.);
    hwca->GetYaxis()->SetTitle("Counts");
    hwca->GetYaxis()->CenterTitle(true);
    hwca->GetYaxis()->SetTitleOffset(1.2);
    TH1D* hwmc = new TH1D("hwmc","WRs: MASTER - CALIFA",300,0.,4500.);
    hwmc->GetYaxis()->SetTitle("Counts");
    hwmc->GetYaxis()->CenterTitle(true);
    hwmc->GetYaxis()->SetTitleOffset(1.2);

    //TREE ENTRIES  ---------------------------------------------------------------
    Long64_t nevents = TWrs->GetEntries();

    Int_t wrmHitsPerEvent=0;
    Int_t wrcHitsPerEvent=0;
    Int_t wraHitsPerEvent=0;
    Int_t difmul=0;
    uint64_t wrm=0,wrc=0,wra=0,firstwrm=0;
    Long64_t nbmasterevents=0;
    Long64_t nbgoodevents=0;
    bool bfirst=true;

    //LOOP IN THE EVENTS  ---------------------------------------------------------
    for(Int_t i=0;i<nevents;i++){
     if(i%500000 == 0) printf("Event: %i\n",i);

     WRMasterCA->Clear();
     WRCalifaCA->Clear();
     WRAmsCA->Clear();
     TWrs->GetEvent(i);
     wrmHitsPerEvent = WRMasterCA->GetEntries(); 
     wrcHitsPerEvent = WRCalifaCA->GetEntries();
     wraHitsPerEvent = WRAmsCA->GetEntries();

     difmul=wrcHitsPerEvent-wrmHitsPerEvent; 

     if(wrmHitsPerEvent>0) {
		wrmasterData = new R3BWRMasterData*[wrmHitsPerEvent];
		for(Int_t j=0;j<wrmHitsPerEvent;j++){
				wrmasterData[j] = new R3BWRMasterData;
				wrmasterData[j] = (R3BWRMasterData*) WRMasterCA->At(j);      
		}
     }

     if(wrcHitsPerEvent>0) {
		wrcalifaData = new R3BWRCalifaData*[wrcHitsPerEvent];
		for(Int_t j=0;j<wrcHitsPerEvent;j++){
				wrcalifaData[j] = new R3BWRCalifaData;
				wrcalifaData[j] = (R3BWRCalifaData*) WRCalifaCA->At(j);      
		}
     }

     if(wraHitsPerEvent>0) {
		wramsData = new R3BWRAmsData*[wraHitsPerEvent];
		for(Int_t j=0;j<wraHitsPerEvent;j++){
				wramsData[j] = new R3BWRAmsData;
				wramsData[j] = (R3BWRAmsData*) WRAmsCA->At(j);      
		}
     }

     //ANALYSIS of the RESULTS  ---------------------------------------------------
     if(wrmHitsPerEvent>0){
     nbmasterevents++;
     hwrmul->Fill(difmul);
     for(Int_t h=0;h<wrmHitsPerEvent;h++){
       wrm=wrmasterData[h]->GetTimeStamp();
       if(bfirst){bfirst=false;firstwrm=wrm;}
       //std::cout<<wrm-firstwrm<<std::endl;
     }
     if(wrcHitsPerEvent>0&&wraHitsPerEvent>0){
     nbgoodevents++;
     for(Int_t h=0;h<wrcHitsPerEvent;h++){
       wrc=wrcalifaData[h]->GetTimeStamp();
       //std::cout<<wrc<<std::endl;
     }
     for(Int_t h=0;h<wraHitsPerEvent;h++){
       wra=wramsData[h]->GetTimeStamp();
       //std::cout<<wra<<std::endl;
     }
      hwmc->Fill(wrm-wrc);
      hwca->Fill(wrc-wra);
     }
      hwrm->Fill((wrm-firstwrm)/1e9);

     }

     if(wrmHitsPerEvent>0)  delete[] wrmasterData;
     if(wrcHitsPerEvent>0)  delete[] wrcalifaData;
     if(wraHitsPerEvent>0)  delete[] wramsData;
    }


    //Draw histograms
    TCanvas* c1 = new TCanvas("WR_Master_califa_ams","WRs",0,0,2000,900);
    c1->Divide(3,1);
    c1->cd(1);
    hwrm->Draw();
    c1->cd(2);
    hwca->Draw();
    c1->cd(3);
    hwmc->Draw();


    // -----   Finish   -------------------------------------------------------
    cout << endl;
    std::cout<<"Total number of events: "<<nevents<<std::endl;
    std::cout<<"Total number of timestamps from the general DAQ: "<<nbmasterevents<<std::endl;
    std::cout<<"Syn. events:"<<nbgoodevents<<std::endl;
    std::cout<<"Only "<<nbgoodevents/(1.0*nbmasterevents)*100.<<" % of syn. events"<<std::endl;
    cout << endl;
}
