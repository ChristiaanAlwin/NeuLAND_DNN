// Include C++ headers:
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

// Include some ROOT-headers:
#include "TString.h"
#include "TROOT.h"
#include "TSystem.h"
#include "TMath.h"
#include "TFile.h"
#include "TH2D.h"
#include "TCanvas.h"

// Include FANN:
#include "fann.h"

// Include own headers:
#include "RoundOff.h"

// Use standard namespaced:
using namespace std;

// Main function definition:
void Validate_Network()
{
    // NOTE: this function will just compute the outcome
    // of the network and do nothing more.
    
    // Specification of parameters:
    TString ValidationFile = "./TestFile.txt";
    TString OutputFile = "./OutputFile.txt";
    Int_t OutputSignificance = 4;
    Bool_t UseHistPlots = kTRUE;
    Double_t PlotsThreshold = 0.0;
    
    // Declaration of Histograms:
    TH2D* InputHist;
    TH2D* OutputHist;
    TH2D* TrueHist;
    TCanvas* c_input;
    TCanvas* c_output;
    TCanvas* c_true;
    
    if (UseHistPlots==kTRUE)
    {
        gROOT->SetBatch(kTRUE);
        InputHist  = new TH2D("InputHist" ,"InputHist" ,60,-150.0,150.0,60,-150.0,150.0);
        OutputHist = new TH2D("OutputHist","OutputHist",60,-150.0,150.0,60,-150.0,150.0);
        TrueHist = new TH2D("TrueHist","TrueHist",60,-150.0,150.0,60,-150.0,150.0);
        c_input = new TCanvas("c_input");
        c_output = new TCanvas("c_output");
        c_true = new TCanvas("c_true");
    }
    
    // Begin by opening the TestFile:
    ifstream InputFile;
    InputFile.open(ValidationFile.Data());
    if (InputFile.is_open())
    {
        // Then, obtain quantities from the first line:
        Int_t nEvents = 0;
        Int_t nInputs = 0;
        Int_t nOutputs = 0;
        InputFile >> nEvents;
        InputFile >> nInputs;
        InputFile >> nOutputs;
        
        // Next, use std::string ThisLine; std::getline(File,ThisLine) to move to other lines.
        
        // Create the outputfile:
        std::ofstream TheTextFile = std::ofstream(OutputFile.Data(), std::ofstream::out);
        
        // Write data:
        TheTextFile << nEvents << " " << nInputs << " " << nOutputs << "\n";
        
        // Then, close the inputfile:
        InputFile.close();
        
        // Next, we must use the neural network:
        struct fann* ann = fann_create_from_file("./TheNetwork.net");
        
        // Next, retrieve the data from the inputsfile with FANN:
        struct fann_train_data* data = fann_read_train_from_file(ValidationFile.Data());
        
        // Next, loop over the number of events & predict outputs with the network:
        cout << "### We start the network computation with nEvents = " << nEvents << " | nInputs = " << nInputs << " | nOutputs = " << nOutputs << "\n\n";
        fann_type* Calculated_Outputs;
        Double_t xpoint;
        Double_t ypoint;
        Double_t zpoint;
        Double_t TAG;
        Double_t TOF;
        Double_t Edep;
        Double_t TheOutput_Calc;
        Double_t TheOutput_True;
        TString st = "";
        TString EventStr = "";
        
        for(Int_t Event = 0; Event<nEvents; ++Event)
        {
            // Define Eventstring:
            EventStr = st.Itoa(Event,10);
            
            // clean the histograms:  
            if (UseHistPlots==kTRUE)
            {                
                for (Int_t kx = 0; kx<52; ++kx)
                {
                    for (Int_t ky = 0; ky<52; ++ky)
                    {
                        InputHist->SetBinContent(kx,ky,0.0);
                        InputHist->SetBinError(kx,ky,0.0);
                        OutputHist->SetBinContent(kx,ky,0.0);
                        OutputHist->SetBinError(kx,ky,0.0);
                        TrueHist->SetBinContent(kx,ky,0.0);
                        TrueHist->SetBinError(kx,ky,0.0);
                    }
                }
            }                    
            
            // Calculate a single event:
            Calculated_Outputs = fann_run(ann, data->input[Event]);
            
            // Next, loop over the signals:
            for (Int_t k = 0; k<nOutputs; ++k)
            {
                // Extract one single signal:
                TAG    = data->input[Event][k*6+0];
                Edep   = data->input[Event][k*6+1];
                TOF    = data->input[Event][k*6+2];
                xpoint = data->input[Event][k*6+3];
                ypoint = data->input[Event][k*6+4];
                zpoint = data->input[Event][k*6+5];
                
                TheOutput_Calc = Calculated_Outputs[k];
                TheOutput_True = data->output[Event][k];
                
                // Do ROOT-validation:
                if (UseHistPlots==kTRUE)
                {
                    // Fill it into the histogram:
                    if (TAG>0.0)
                    {
                        InputHist->Fill(xpoint,ypoint);
                        
                        if (TheOutput_Calc>PlotsThreshold)
                        {
                            OutputHist->Fill(xpoint,ypoint);
                            
                            // Give screen output:
                            if (Event==0) {cout << TheOutput_Calc << " | " << TheOutput_True << "\n";}
                        }
                        
                        if (TheOutput_True>0.99)
                        {
                            TrueHist->Fill(xpoint,ypoint);
                        }
                    }
                }
                
                // Write output to a file:
                TheTextFile << RoundOff(TheOutput_Calc,OutputSignificance).Data() << " ";
            }
            
            // Put a new line in the output-file:
            TheTextFile << "\n";
            
            // Make plots:
            if (UseHistPlots==kTRUE)
            {
                c_input->cd();
                InputHist->Draw("colz");
                c_input->SaveAs("./Plots/InputHist"+EventStr+".png");
                
                c_output->cd();
                OutputHist->Draw("colz");
                c_output->SaveAs("./Plots/OutputHist"+EventStr+".png");
                
                c_true->cd();
                TrueHist->Draw("colz");
                c_true->SaveAs("./Plots/TrueHist"+EventStr+".png");
            }
            
            // Give a sign of life:
            if ((Event+1)%1==0) {cout << "The Network computed " << Event+1 << "\n";}
            
            // Done.
        }
        
        // Close the output file:
        TheTextFile.close();
    }
    else
    {
        cout << "### ERROR: The specified validation-file does not exist!\n\n";
    }
}

// Define the main for stand-alone compilation:
#ifndef __CINT__
#include <TApplication.h>
int main(int argc, char** argv)
{
    TApplication* TheApp = new TApplication("Validate_Network",&argc,argv);
    Validate_Network();
    // TheApp->Run(); ==> This macro does not contain graphics.
    return 0;
}
#endif

