// Include C++ headers:
#include <iostream>
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

// Use standard namespaced:
using namespace std;

// Definition of a single training-step:
void OneEpoch(Int_t const nEpochsPerRun)
{
    // NOTE: This function will handle the training of the network.
    // It will first load the network from a file, then it will
    // execute a some epoch, and then it will overwrite the 
    // network. With this structure, we can interrupt the training
    // at any desired time, even though we are only half-way.
    TString TrainingFile = "../../TrainingSets/Baby/FANN_Training.txt";
    Double_t DesiredError = 1e-6;
    
    // Load the network:
    cout << "==> ### DO NOT INTERRUPT NOW! ###\n";
    struct fann* ann = fann_create_from_file("./TheNetwork.net");
    cout << "You may interrup if you like...\n";
    
    // Next, execute a training:
    // First = total number of epochs, second is #epochs after which to show a message.
    fann_train_on_file(ann,TrainingFile.Data(),nEpochsPerRun,1,DesiredError);
    
    // Next, save the network again:
    cout << "==> ### DO NOT INTERRUPT NOW! ###\n";
    fann_save(ann,"./TheNetwork.net");
    fann_destroy(ann);
    cout << "You may interrup if you like...\n";
    
    // Done.
}

// Loop-definition:
void Train_Network(Int_t const nInteruptions, Int_t const nEpochsPerRun)
{
    cout << "### Executing Network training...\n"
         << "Total number of epochs = " << nInteruptions*nEpochsPerRun << "\n"
         << "Number of epochs per saving = " << nEpochsPerRun << "\n\n";
    // Execute training:
    for (Int_t k = 0; k<nInteruptions; ++k)
    {
        cout << "===> Saving round = " << k << "/" << nInteruptions << "\n";
        OneEpoch(nEpochsPerRun);
        cout << "\n\n";
    }
    
    // Done.
}

// Define the main for stand-alone compilation:
#ifndef __CINT__
#include <TApplication.h>
int main(int argc, char** argv)
{
    TString Interruptions_str = argv[1];  
    TString EpochsPerInter_str = argv[2];
    
    if ((Interruptions_str.IsDigit()==kFALSE)||(EpochsPerInter_str.IsDigit()==kFALSE))
    {
        cout << "You should supply only integers as input arguments!\n\n";
    }
    else
    {
        Int_t nInters = Interruptions_str.Atoi();
        Int_t nEpochs = EpochsPerInter_str.Atoi();
        
        TApplication* TheApp = new TApplication("Train_Network",&argc,argv);
        Train_Network(nInters,nEpochs);
        // TheApp->Run(); ==> This macro does not contain graphics.
    }
    return 0;
}
#endif
