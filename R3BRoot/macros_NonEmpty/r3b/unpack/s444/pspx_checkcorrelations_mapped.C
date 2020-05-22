#include <TROOT.h>
#include "TApplication.h"
#include "TSystem.h"
#include "TStyle.h"
#include "TMath.h"
#include <TChain.h>
#include <TFile.h>
#include "TObject.h"
#include "TClonesArray.h"
#include "TFitResult.h"
#include "TMatrixTSym.h"

#include "TCanvas.h"
#include "TSpectrum.h"
#include "TGraph.h"
#include "TH1F.h"
#include "TH2F.h"

#include "TF1.h"
#include "TLatex.h"

#include "FairEventHeader.h"
#include "/home/land/R3BRoot_PSPX/r3bbase/R3BEventHeader.h"

#include "Riostream.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>

using namespace std;
using namespace TMath;

void pspx_checkcorrelations_mapped(UInt_t run_min = 11, UInt_t run_max=13,UInt_t strip_f1=16, UInt_t strip_b1=48, UInt_t strip_f2=16, UInt_t strip_b2=48, UInt_t strip_f3=16, UInt_t strip_b3=48){
	
	UInt_t verbosity_step_size=1000000; //number, how often progress of skript is reported
	
	//Detector specific variables
	UInt_t nStrips=32;
	UInt_t npeaks=nStrips-1;
	
	UInt_t selectReadout=0; //0=energy,1=position
	Double_t minE=0;
	Double_t maxE=500000;
	Double_t minPos = -0.5;
	Double_t maxPos = 0.5;
	Double_t threshold = 5000.;
	Double_t maxE_interstrip = 380000.;
	Double_t neve=1.;
	
	//Input files  
	TChain *fChain = new TChain("evt");
	TString filename;
	for(UInt_t i=run_min; i<=run_max; i++){
		//filename = Form("/home/land/PSPX/rootfiles_tests/s473_tests/pspx_run0%03d_mapped.root", i);
		filename = Form("/home/land/PSPX/rootfiles_tests/s473_tests/pspx_run0%03d_cal.root", i);
		cout << "Open " << filename << endl;
		fChain->Add(filename); 
	}
	
	#include "test_6det.h"
	fChain->SetMakeClass(1);
	
	// --- HISTOGRAMS ---     
	UInt_t binE=1000;
	UInt_t binPos=1000;
	
	TH2F* HPsp1_enb_posf=new TH2F("HPsp1_enb_posf","HPsp1_enb_posf; position front; energy back", binPos,minPos,maxPos,binE,-maxE,-minE);
	TH2F* HPsp1_enf_posb=new TH2F("HPsp1_enf_posb","HPsp1_enf_posb; position back; energy front", binPos,minPos,maxPos,binE,minE,maxE);
	TH2F* HPsp1_enf_enb=new TH2F("HPsp1_enf_enb","HPsp1_enf_enb; energy back; energy front", binE,-maxE,-minE,binE,minE,maxE);
	TH2F* HPsp1_sumen_neve=new TH2F("HPsp1_sumen_neve","HPsp1_sumen_neve; position back; position front", binPos,minPos,maxPos,binPos,minPos,maxPos);
	TH1F* HPsp1_pos_back = new TH1F("HPsp1_pos_back","HPsp1_pos_back; position back in a.u.",binPos,minPos,maxPos);
	TH1F* HPsp1_pos_front = new TH1F("HPsp1_pos_front","HPsp1_pos_front; position front in a.u.",binPos,minPos,maxPos);
	TH2F* HPsp1_interstrip = new TH2F("HPsp1_interstrip","HPsp1_interstrip; position back; position front", binPos,minPos,maxPos,binPos,minPos,maxPos);
	TH2F* HPsp1_interstrip_eb= new TH2F("HPsp1_interstrip_eb","HPsp1_interstrip_eb; energy strip 1; energy strip 2", binE,-maxE,-minE, binE,-maxE,-minE);
	TH2F* HPsp1_interstrip_ef= new TH2F("HPsp1_interstrip_ef","HPsp1_interstrip_ef; energy strip 1; energy strip 2", binE,minE,maxE,binE,minE,maxE);
	
	TH2F* HPsp2_enb_posf=new TH2F("HPsp2_enb_posf","HPsp2_enb_posf; position front; energy back", binPos,minPos,maxPos,binE,minE,maxE);
	TH2F* HPsp2_enf_posb=new TH2F("HPsp2_enf_posb","HPsp2_enf_posb; position back; energy front", binPos,minPos,maxPos,binE,-maxE,minE);
	TH2F* HPsp2_enf_enb=new TH2F("HPsp2_enf_enb","HPsp2_enf_enb; energy back; energy front", binE,minE,maxE,binE,-maxE,minE);
	TH2F* HPsp2_sumen_neve=new TH2F("HPsp2_sumen_neve","HPsp2_sumen_neve; position back; position front", binPos,minPos,maxPos, binPos, minPos, maxPos);
	TH1F* HPsp2_pos_back = new TH1F("HPsp2_pos_back","HPsp2_pos_back; position back in a.u.",binPos,minPos,maxPos);
	TH1F* HPsp2_pos_front = new TH1F("HPsp2_pos_front","HPsp2_pos_front; position front in a.u.",binPos,minPos,maxPos);
	TH2F* HPsp2_interstrip = new TH2F("HPsp2_interstrip","HPsp2_interstrip; position back; position front", binPos,minPos,maxPos,binPos,minPos,maxPos);
	TH2F* HPsp2_interstrip_eb= new TH2F("HPsp2_interstrip_eb","HPsp2_interstrip_eb; energy strip 1; energy strip 2", binE,minE,maxE,binE,minE,maxE);
	TH2F* HPsp2_interstrip_ef= new TH2F("HPsp2_interstrip_ef","HPsp2_interstrip_ef; energy strip 1; energy strip 2", binE,-maxE,-minE,binE,-maxE,-minE);
	
	TH2F* HPsp3_enb_posf=new TH2F("HPsp3_enb_posf","HPsp3_enb_posf; position front; energy back", binPos,minPos,maxPos,binE,-maxE,-minE);
	TH2F* HPsp3_enf_posb=new TH2F("HPsp3_enf_posb","HPsp3_enf_posb; position back; energy front", binPos,minPos,maxPos,binE,minE,maxE);
	TH2F* HPsp3_enf_enb=new TH2F("HPsp3_enf_enb","HPsp3_enf_enb; energy back; energy front", binE,-maxE,-minE,binE,minE,maxE);
	TH2F* HPsp3_sumen_neve=new TH2F("HPsp3_sumen_neve","HPsp3_sumen_neve; position back; position front", binPos,minPos,maxPos,binPos,minPos,maxPos);
	TH1F* HPsp3_pos_back = new TH1F("HPsp3_pos_back","HPsp3_pos_back; position back in a.u.", binPos,minPos,maxPos);
	TH1F* HPsp3_pos_front = new TH1F("HPsp3_pos_front","HPsp3_pos_front; position front in a.u.", binPos,minPos,maxPos);
	TH2F* HPsp3_interstrip = new TH2F("HPsp3_interstrip","HPsp3_interstrip; position back; position front", binPos,minPos,maxPos,binPos,minPos,maxPos);
	TH2F* HPsp3_interstrip_eb= new TH2F("HPsp3_interstrip_eb","HPsp3_interstrip_eb; energy strip 1; energy strip 2", binE,-maxE,-minE,binE,-maxE,-minE);
	TH2F* HPsp3_interstrip_ef= new TH2F("HPsp3_interstrip_ef","HPsp3_interstrip_ef; energy strip 1; energy strip 2", binE,minE,maxE,binE,minE,maxE);
	
	TH2F* H_psp12 = new TH2F("H_psp12","H_psp12; energy psp1; energy psp2", binE,minE,maxE,binE,-maxE,-minE);
	TH2F* H_psp23 = new TH2F("H_psp23","H_psp23; energy psp2; energy psp3", binE,minE,maxE,binE,-maxE,-minE);
	TH2F* H_psp23sum = new TH2F("H_psp23sum","H_psp23sum; energy psp2+psp3", binE,-0.2*maxE,0.2*maxE,binE,-2*maxE,0);
	
	std::vector<Double_t> det;
	std::vector<Double_t> pos_front1, pos_back1;
	std::vector<Double_t> pos_front2, pos_back2;
	std::vector<Double_t> pos_front3, pos_back3;
	std::vector<Double_t> en_front1, en_back1;
	std::vector<Double_t> en_front2, en_back2;
	std::vector<Double_t> en_front3, en_back3;
	std::vector<Double_t> strip_front1, strip_back1;
	std::vector<Double_t> strip_front2, strip_back2;
	std::vector<Double_t> strip_front3, strip_back3;


  // --- EVENTLOOP ---
	if (fChain == 0) return;
	Long64_t nentries = fChain->GetEntries();
	Long64_t nbytes = 0, nb = 0;
	
	Int_t mult_front1, mult_back1;
	Int_t mult_front2, mult_back2;
	Int_t mult_front3, mult_back3;
	
	Int_t counter_front1=0, counter_front2=0, counter_front3=0;
	Int_t counter_back1=0, counter_back2=0, counter_back3=0;
	
	fChain->SetBranchStatus("*",0);
	fChain->SetBranchStatus("PspxCalData*",1);
	fChain->SetBranchStatus("R3BEventHeader*",1);
	
	for (Long64_t jentry=0; jentry<neve*nentries;jentry++){
		Long64_t ientry = fChain->LoadTree(jentry);
		if (ientry < 0) break;
			
		nb = fChain->GetEntry(jentry);
		//cout << nb<<" "<< ientry << " " << jentry<< endl;
		nbytes += nb;
		if(nb==4) continue;
		if(nb==0) {cout<< "Something went terribly wrong!" << endl; break;};
		
		if (jentry%verbosity_step_size==0){
			  cout << jentry << " of " << nentries << " == "<< 100. * jentry / nentries <<"% of the events analyzed\r"<< flush;
		}
		
		  // Reseting all values
		mult_front1=0;
		mult_back1=0;
		mult_front2=0;
		mult_back2=0;
		mult_front3=0;
		mult_back3=0;
		pos_front1.clear();
		pos_front2.clear();
		pos_front3.clear();
		pos_back1.clear();
		pos_back2.clear();
		pos_back3.clear();
		en_front1.clear();
		en_front2.clear();
		en_front3.clear();
		en_back1.clear();
		en_back2.clear();
		en_back3.clear();
		strip_front1.clear();
		strip_front2.clear();
		strip_front3.clear();
		strip_back1.clear();
		strip_back2.clear();
		strip_back3.clear();
		det.clear();
		
		//Get multiplicity and values for every detector
		for(Int_t i=0; i<PspxCalData_;i++){
			if(PspxCalData_fStrip[i]>0 && PspxCalData_fStrip[i]<nStrips+1){
		      		if(PspxCalData_fDetector[i] == 1+selectReadout){
					if (!isnan(PspxCalData_fEnergy1[i]) && !isnan(PspxCalData_fEnergy2[i])) {
						if (abs(PspxCalData_fEnergy1[i]) > threshold && abs(PspxCalData_fEnergy2[i]) > threshold) {
							mult_front1++;
					 		en_front1.push_back(PspxCalData_fEnergy1[i] + PspxCalData_fEnergy2[i]); //energy front
					 		pos_front1.push_back((PspxCalData_fEnergy1[i] - PspxCalData_fEnergy2[i])/(PspxCalData_fEnergy1[i] + PspxCalData_fEnergy2[i])); //position front
							strip_front1.push_back(PspxCalData_fStrip[i]);
							det.push_back(PspxCalData_fDetector[i]);
						}
					}
				}
				if(PspxCalData_fDetector[i] == 3+selectReadout){
					mult_front2++;
					if (!isnan(PspxCalData_fEnergy1[i]) && !isnan(PspxCalData_fEnergy2[i])) {
						if (abs(PspxCalData_fEnergy1[i]) > threshold && abs(PspxCalData_fEnergy2[i]) > threshold) {
					 		en_front2.push_back(PspxCalData_fEnergy1[i] + PspxCalData_fEnergy2[i]); //energy front
					 		pos_front2.push_back((PspxCalData_fEnergy1[i] - PspxCalData_fEnergy2[i])/(PspxCalData_fEnergy1[i] + PspxCalData_fEnergy2[i])); //position front
							strip_front2.push_back(PspxCalData_fStrip[i]);
							det.push_back(PspxCalData_fDetector[i]);
						}
					}
				}
				if(PspxCalData_fDetector[i] == 5+selectReadout){
					mult_front3++;
					if (!isnan(PspxCalData_fEnergy1[i]) && !isnan(PspxCalData_fEnergy2[i])) {
						if (abs(PspxCalData_fEnergy1[i]) > threshold && abs(PspxCalData_fEnergy2[i]) > threshold) {
							en_front3.push_back(PspxCalData_fEnergy1[i] + PspxCalData_fEnergy2[i]); //energy front
							pos_front3.push_back((PspxCalData_fEnergy1[i] - PspxCalData_fEnergy2[i])/(PspxCalData_fEnergy1[i] + PspxCalData_fEnergy2[i])); //position front
							strip_front3.push_back(PspxCalData_fStrip[i]);
							det.push_back(PspxCalData_fDetector[i]);
						}
					}
				}
			}
			else if(PspxCalData_fStrip[i]>nStrips && PspxCalData_fStrip[i]<2*nStrips+1){
				if(PspxCalData_fDetector[i] == 1+selectReadout){
					mult_back1++;
					if (!isnan(PspxCalData_fEnergy1[i]) && !isnan(PspxCalData_fEnergy2[i])) {
						if (abs(PspxCalData_fEnergy1[i]) > threshold && abs(PspxCalData_fEnergy2[i]) > threshold) {
					 		en_back1.push_back(PspxCalData_fEnergy1[i] + PspxCalData_fEnergy2[i]); //energy back
					 		pos_back1.push_back((PspxCalData_fEnergy1[i] - PspxCalData_fEnergy2[i])/(PspxCalData_fEnergy1[i] + PspxCalData_fEnergy2[i])); //position back
							strip_back1.push_back(PspxCalData_fStrip[i]);
							det.push_back(PspxCalData_fDetector[i]);
						}
					}
				}
				if(PspxCalData_fDetector[i] == 3+selectReadout){
					mult_back2++;
					if (!isnan(PspxCalData_fEnergy1[i]) && !isnan(PspxCalData_fEnergy2[i])) {
						if (abs(PspxCalData_fEnergy1[i]) > threshold && abs(PspxCalData_fEnergy2[i]) > threshold) {
					 		en_back2.push_back(PspxCalData_fEnergy1[i] + PspxCalData_fEnergy2[i]); //energy back
					 		pos_back2.push_back((PspxCalData_fEnergy1[i] - PspxCalData_fEnergy2[i])/(PspxCalData_fEnergy1[i] + PspxCalData_fEnergy2[i])); //position back
							strip_back2.push_back(PspxCalData_fStrip[i]);
							det.push_back(PspxCalData_fDetector[i]);
						}
					}
				}
				if(PspxCalData_fDetector[i] == 5+selectReadout){
					mult_back3++;
					if (!isnan(PspxCalData_fEnergy1[i]) && !isnan(PspxCalData_fEnergy2[i])) {
						if (abs(PspxCalData_fEnergy1[i]) > threshold && abs(PspxCalData_fEnergy2[i]) > threshold) {
					 		en_back3.push_back(PspxCalData_fEnergy1[i] + PspxCalData_fEnergy2[i]); //energy back
					 		pos_back3.push_back((PspxCalData_fEnergy1[i] - PspxCalData_fEnergy2[i])/(PspxCalData_fEnergy1[i] + PspxCalData_fEnergy2[i])); //position back
							strip_back3.push_back(PspxCalData_fStrip[i]);
							det.push_back(PspxCalData_fDetector[i]);
						}
					}
				}
			}
		}
		
		if(mult_front1==2 && mult_back1==1) counter_front1++; //was swapped to count interstrip events 
		else if(mult_front1==1 && mult_back1==2) counter_back1++;
		if(mult_front2==2 && mult_back2==1) counter_front2++; //was swapped to count interstrip events 
		else if(mult_front2==1 && mult_back2==2) counter_back2++;
		if(mult_front3==2 && mult_back3==1) counter_front3++; //was swapped to count interstrip events 
		else if(mult_front3==1 && mult_back3==2) counter_back3++;
		
		//Correlation Energy back vs pos front 
		if (mult_back1 == 1 && mult_front1==1) {      
			HPsp1_enb_posf->Fill(pos_front1[0],en_back1[0]);
			HPsp1_enf_posb->Fill(pos_back1[0],en_front1[0]);
			HPsp1_enf_enb->Fill(en_back1[0],en_front1[0]);
			HPsp1_sumen_neve->Fill(pos_back1[0],pos_front1[0]);
			HPsp1_pos_back->Fill(pos_back1[0]);
			HPsp1_pos_front->Fill(pos_front1[0]);
		}
		if (mult_back2 == 1 && mult_front2==1) {      
		 	HPsp2_enb_posf->Fill(pos_front2[0],en_back2[0]);
		 	HPsp2_enf_posb->Fill(pos_back2[0],en_front2[0]);
		 	HPsp2_enf_enb->Fill(en_back2[0],en_front2[0]);
			HPsp2_sumen_neve->Fill(pos_back2[0],pos_front2[0]);
			HPsp2_pos_back->Fill(pos_back2[0]);
			HPsp2_pos_front->Fill(pos_front2[0]);
		}
		if (mult_back3 == 1 && mult_front3==1) {      
			HPsp3_enb_posf->Fill(pos_front3[0],en_back3[0]);
		 	HPsp3_enf_posb->Fill(pos_back3[0],en_front3[0]);
		 	HPsp3_enf_enb->Fill(en_back3[0],en_front3[0]);
			HPsp3_sumen_neve->Fill(pos_back3[0],pos_front3[0]);
			HPsp3_pos_back->Fill(pos_back3[0]);
			HPsp3_pos_front->Fill(pos_front3[0]);
		}
		
		//Energy correlation PSP 2 vs PSP 1 
		if (mult_back1==1 && mult_front1==1 && mult_back3==1 && mult_front3==1) {      
			if(strip_front1[0] == strip_f1 && strip_front2[0]==strip_f2){ 
				H_psp12->Fill(en_front1[0],en_front2[0]);
			}
		}
			
		//Energy correlation PSP 3 vs PSP 2 
		if (mult_back2==1 && mult_front2==1 && mult_back3==1 && mult_front3==1) {      
			if(strip_back2[0] == strip_b2 && strip_back3[0]==strip_b3){
				H_psp23->Fill(en_back2[0],en_back3[0]);
				H_psp23sum->Fill(en_back2[0]+en_back3[0],-en_back2[0]+en_back3[0]);
			}
		}
			
		// Interstrip events PSP 1
		if ((mult_back1== 2 && mult_front1==1 && abs(en_back1[0]+en_back1[1])<maxE_interstrip) || (mult_back1 == 1 && mult_front1==2 && abs(en_front1[0]+en_front1[1])<maxE_interstrip)) {
		     HPsp1_interstrip->Fill(pos_back1[0],pos_front1[0]);
		}
			
		if (mult_back1== 1 && mult_front1==2) {
		     HPsp1_interstrip_eb->Fill(en_back1[1],en_back1[0]);
		}

		if (mult_back1== 2 && mult_front1==1) {
		     HPsp1_interstrip_ef->Fill(en_front1[1],en_front1[0]);
		}
		
		// Interstrip events PSP 2
		if ((mult_back2== 2 && mult_front2==1 && abs(en_back2[0]+en_back2[1])<maxE_interstrip) || (mult_back2 == 1 && mult_front2==2 && abs(en_front2[0]+en_front2[1])<maxE_interstrip)) {
		     HPsp2_interstrip->Fill(pos_back2[0],pos_front2[0]);
		}
		
		if (mult_back2== 2 && mult_front2==1) {
		     HPsp2_interstrip_eb->Fill(en_back2[1],en_back2[0]);
		}

		if (mult_back2== 1 && mult_front2==2) {
		     HPsp2_interstrip_ef->Fill(en_front2[1],en_front2[0]);
		}

		// Interstrip events PSP 3
		if ((mult_back3== 2 && mult_front3==1 && abs(en_back3[0]+en_back3[1])<maxE_interstrip) || (mult_back3 == 1 && mult_front3==2 && abs(en_front3[0]+en_front3[1])<maxE_interstrip)) {
		     HPsp3_interstrip->Fill(pos_back3[0],pos_front3[0]);
		}	
		
		if (mult_back3== 2 && mult_front3==1) {
		     HPsp3_interstrip_eb->Fill(en_back3[1],en_back3[0]);
		}

		if (mult_back3== 1 && mult_front3==2) {
		     HPsp3_interstrip_ef->Fill(en_front3[1],en_front3[0]); 
		}

	} // end of event loop

	//Fits 
	
	Double_t max1=0, max2=0, max3=0;
	UInt_t firstbin1=0, firstbin2=0, firstbin3=0;
	Double_t firstmean1=0, firstmean2=0, firstmean3=0;
	Double_t fitrange1=0.002, fitrange2=0.002, fitrange3=0.002;
			
	max1 = HPsp1_pos_back->GetMaximum();
	firstbin1 = HPsp1_pos_back->FindFirstBinAbove(max1/2);
	firstmean1 = HPsp1_pos_back->GetBinCenter(firstbin1);
	TF1 * gaus_back1 = new TF1("gaus_back1","gaus(0)",-0.2,0.2);
	HPsp1_pos_back->Fit("gaus_back1", "", "B0+", firstmean1-2*fitrange1, firstmean1+fitrange1);
	cout << "back1 first: " << firstmean1 << ", " << gaus_back1->GetParameter(2) << ", " << gaus_back1->GetParameter(2)*100000./2 << " um " << endl;
	
	max1 = HPsp1_pos_front->GetMaximum();
	firstbin1 = HPsp1_pos_front->FindFirstBinAbove(max1/2);
	firstmean1 = HPsp1_pos_front->GetBinCenter(firstbin1);
	TF1 * gaus_front1 = new TF1("gaus_front1","gaus(0)",-0.2,0.2);
	HPsp1_pos_front->Fit("gaus_front1", "", "B0+", firstmean1-2*fitrange1, firstmean1+fitrange1);
	cout << "front1 first: " << firstmean1 << ", " << gaus_front1->GetParameter(2) << ", " << gaus_front1->GetParameter(2)*100000./2 << " um " << endl;

	max2 = HPsp2_pos_back->GetMaximum();
	firstbin2 = HPsp2_pos_back->FindFirstBinAbove(max2/2);
	firstmean2 = HPsp2_pos_back->GetBinCenter(firstbin2);
	TF1 * gaus_back2 = new TF1("gaus_back2","gaus(0)",-0.2,0.2);
	HPsp2_pos_back->Fit("gaus_back2", "", "B0+", firstmean2-2*fitrange2, firstmean2+fitrange2);
	cout << "back2 first: " << firstmean2 << ", " << gaus_back2->GetParameter(2) << ", " << gaus_back2->GetParameter(2)*100000./2 << " um " << endl;
	
	max2 = HPsp2_pos_front->GetMaximum();
	firstbin2 = HPsp2_pos_front->FindFirstBinAbove(max2/2);
	firstmean2 = HPsp2_pos_front->GetBinCenter(firstbin2);
	TF1 * gaus_front2 = new TF1("gaus_front2","gaus(0)",-0.2,0.2);
	HPsp2_pos_front->Fit("gaus_front2", "", "B0+", firstmean2-2*fitrange2, firstmean2+fitrange2);
	cout << "front2 first: " << firstmean2 << ", " << gaus_front2->GetParameter(2) << ", " << gaus_front2->GetParameter(2)*100000./2 << " um " << endl;

	max3 = HPsp3_pos_back->GetMaximum();
	firstbin3 = HPsp3_pos_back->FindFirstBinAbove(max3/2);
	firstmean3 = HPsp3_pos_back->GetBinCenter(firstbin3);
	TF1 * gaus_back3 = new TF1("gaus_back3","gaus(0)",-0.2,0.2);
	HPsp3_pos_back->Fit("gaus_back3", "", "B0+", firstmean3-2*fitrange3, firstmean3+fitrange3);
	cout << "back3 first: " << firstmean3 << ", " << gaus_back3->GetParameter(2) << ", " << gaus_back3->GetParameter(2)*100000./2 << " um " << endl;
	
	max3 = HPsp3_pos_front->GetMaximum();
	firstbin3 = HPsp3_pos_front->FindFirstBinAbove(max3/2);
	firstmean3 = HPsp3_pos_front->GetBinCenter(firstbin3);
	TF1 * gaus_front3 = new TF1("gaus_front3","gaus(0)",-0.2,0.2);
	HPsp3_pos_front->Fit("gaus_front3", "", "B0+", firstmean3-2*fitrange3, firstmean3+fitrange3);
	cout << "front3 first: " << firstmean3 << ", " << gaus_front3->GetParameter(2) << ", " << gaus_front3->GetParameter(2)*100000./2 << " um " << endl;
	
	cout << "\nCounter PSP 1 front interstrip: " << counter_front1 << endl;
	cout << "Counter PSP 1 back interstrip: " << counter_back1 << endl;
	cout << "\nCounter PSP 2 front interstrip: " << counter_front2 << endl;
	cout << "Counter PSP 2 back interstrip: " << counter_back2 << endl;
	cout << "\nCounter PSP 3 front interstrip: " << counter_front3 << endl;
	cout << "Counter PSP 3 back interstrip: " << counter_back3 << endl;
	
	//Plots
	TCanvas * cPsp1_evsp = new TCanvas("cPsp1_evsp","PSP 1 Energy vs Position",1200,800);
	cPsp1_evsp->Divide(2,2);
	cPsp1_evsp->cd(1);
	HPsp1_enb_posf->Draw("colz");
	cPsp1_evsp->cd(2);
	HPsp1_enf_posb->Draw("colz");
	cPsp1_evsp->cd(3);
	HPsp1_enf_enb->Draw("colz");
	//cPsp1_evsp->cd(4);
	//HPsp1_sumen_neve->Draw("colz");
	
	TCanvas * cPsp1_pos = new TCanvas("cPsp1_pos","PSP 1 Position check",1200,800);
	cPsp1_pos->Divide(2,2);
	cPsp1_pos->cd(1);
	HPsp1_pos_back->Draw();
	cPsp1_pos->cd(2);
	HPsp1_pos_front->Draw();
	cPsp1_pos->cd(3);
	HPsp1_sumen_neve->Draw("colz");
	cPsp1_pos->cd(4);
	HPsp1_interstrip->Draw("colz");
	
	//Plots
	TCanvas * cPsp2_evsp = new TCanvas("cPsp2_evsp","PSP 2 Energy vs Position",1200,800);
	cPsp2_evsp->Divide(2,2);
	cPsp2_evsp->cd(1);
	HPsp2_enb_posf->Draw("colz");
	cPsp2_evsp->cd(2);
	HPsp2_enf_posb->Draw("colz");
	cPsp2_evsp->cd(3);
	HPsp2_enf_enb->Draw("colz");
	cPsp2_evsp->cd(4);
	
	TCanvas * cPsp2_pos = new TCanvas("cPsp2_pos","PSP 2 Position check",1200,800);
	cPsp2_pos->Divide(2,2);
	cPsp2_pos->cd(1);
	HPsp2_pos_back->Draw();
	cPsp2_pos->cd(2);
	HPsp2_pos_front->Draw();
	cPsp2_pos->cd(3);
	HPsp2_sumen_neve->Draw("colz");
	cPsp2_pos->cd(4);
	HPsp2_interstrip->Draw("colz");
	
	//Plots
	TCanvas * cPsp3_evsp = new TCanvas("cPsp3_evsp","PSP 3 Energy vs Position",1200,800);
	cPsp3_evsp->Divide(2,2);
	cPsp3_evsp->cd(1);
	HPsp3_enb_posf->Draw("colz");
	cPsp3_evsp->cd(2);
	HPsp3_enf_posb->Draw("colz");
	cPsp3_evsp->cd(3);
	HPsp3_enf_enb->Draw("colz");
	cPsp3_evsp->cd(4);
	
	TCanvas * cPsp3_pos = new TCanvas("cPsp3_pos","PSP 3 Position check",1200,800);
	cPsp3_pos->Divide(2,2);
	cPsp3_pos->cd(1);
	HPsp3_pos_back->Draw();
	cPsp3_pos->cd(2);
	HPsp3_pos_front->Draw();
	cPsp3_pos->cd(3);
	HPsp3_sumen_neve->Draw("colz");
	cPsp3_pos->cd(4);
	HPsp3_interstrip->Draw("colz");
	
	
	TCanvas * cE_interstrip = new TCanvas("cE_interstrip","Energies interstrip events",1200,800);
	cE_interstrip->Divide(2,3);
	cE_interstrip->cd(1);
	HPsp1_interstrip_ef->Draw("colz");
	cE_interstrip->cd(2);
	HPsp1_interstrip_eb->Draw("colz");
	cE_interstrip->cd(3);
	HPsp2_interstrip_ef->Draw("colz");
	cE_interstrip->cd(4);
	HPsp2_interstrip_eb->Draw("colz");
	cE_interstrip->cd(5);
	HPsp3_interstrip_ef->Draw("colz");
	cE_interstrip->cd(6);
	HPsp3_interstrip_eb->Draw("colz");

	
	TCanvas * cEcorr = new TCanvas("cEcorr","PSP corr check",1200,800);
	cEcorr->Divide(1,2);
	cEcorr->cd(1);
	H_psp23sum->Draw("colz");
	cEcorr->cd(2);
	H_psp23->Draw("colz");
}







