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
    TString ValidationFile = "./DNN_InputFile.txt";
    TString OutputFile = "./OutputFile.txt";
    Int_t OutputSignificance = 4;
    Int_t MaxMult = 5;
    
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
        Int_t* True_MultCounters = new Int_t[MaxMult+1];
        Int_t* Pred_MultCounters = new Int_t[MaxMult+1];
        Int_t* Coin_MultCounters = new Int_t[MaxMult+1];
        Double_t* Percentages = new Double_t[MaxMult+1];
        Int_t TrueIndex = -1;
        Int_t PredIndex = -1;
        Double_t TrueMax = -1000.0;
        Double_t PredMax = -1000.0;
        Double_t TrueOutput = 0.0;
        Double_t PredOutput = 0.0;
        TString st = "";
        TString EventStr = "";
        
        for (Int_t k = 0; k<(MaxMult+1); ++k)
        {
            True_MultCounters[k] = 0;
            Pred_MultCounters[k] = 0;
            Coin_MultCounters[k] = 0;
        }
        
        for(Int_t Event = 0; Event<nEvents; ++Event)
        {
            // Define Eventstring:
            EventStr = st.Itoa(Event,10);
            
            // Calculate a single event:
            Calculated_Outputs = fann_run(ann, data->input[Event]);
            
            // Next, loop over the outputs to find the maximum:
            TrueIndex = -1;
            PredIndex = -1;
            TrueMax = -1000.0;
            PredMax = -1000.0;
            TrueOutput = 0.0;
            PredOutput = 0.0;
            
            for (Int_t k = 0; k<(MaxMult+1); ++k)
            {
                // Obtain numbers:
                TrueOutput = data->output[Event][k];
                PredOutput = Calculated_Outputs[k];
                
                // Compare:
                if (TrueOutput>TrueMax)
                {
                    TrueMax = TrueOutput;
                    TrueIndex = k;
                }
                
                if (PredOutput>PredMax)
                {
                    PredMax = PredOutput;
                    PredIndex = k;
                }
                
                // Write output to file:
                TheTextFile << RoundOff(PredOutput,OutputSignificance).Data() << " ";
            }
                
            // Put a new line in the output-file:
            TheTextFile << "\n";
            
            // Update the counters:
            True_MultCounters[TrueIndex] = True_MultCounters[TrueIndex] + 1;
            Pred_MultCounters[PredIndex] = Pred_MultCounters[PredIndex] + 1;
            if (PredIndex==TrueIndex) {Coin_MultCounters[PredIndex] = Coin_MultCounters[PredIndex] + 1;}
            
            // Give a sign of life:
            if ((Event+1)%1==0) {cout << "The Network computed " << Event+1 << "\n";}
            
            // Done.
        }
        
        // Give results:
        cout << "\n\n";
        
        for (Int_t k = 0; k<(MaxMult+1); ++k)
        {
            Percentages[k] = ((Int_t) Coin_MultCounters[k]);
            Percentages[k] = Percentages[k]/((Int_t) True_MultCounters[k]);
            cout << k << "n multiplicity: #events: correctly pred./all true events = " << Coin_MultCounters[k] << "/" << True_MultCounters[k] << " = " << Percentages[k] << "\n";
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

