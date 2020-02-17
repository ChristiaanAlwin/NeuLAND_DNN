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

// Main function definition:
void Design_Network()
{
    // NOTE: This function only designs the network.
    // No training or what so-ever is done
    // (although the training properties are set).
    cout << "\n\n==> Designing the network...\n\n";

    // Define network parameters:
    Int_t nLayers = 5;
    Int_t nInputs = 600;
    Int_t nOutputs = 100;
    Double_t Activation_Steepness = 0.1;
    Double_t LearningRate = 0.1;
    
    // Define Network configuration:
    Int_t nHidden = nInputs;
    if (nHidden<nOutputs) {nHidden = nOutputs;}
    
    Int_t* NeuronArray = new Int_t[nLayers];
    
    for (Int_t k = 0; k<nLayers; ++k)
    {
        if (k==0) {NeuronArray[k] = nInputs;}
        else if (k==(nLayers-1)) {NeuronArray[k] = nOutputs;}
        else {NeuronArray[k] = nHidden;}
    }
    
    cout << "Parameters were specified.\n";
    
    // Next, create the blank network:
    struct fann* ann = fann_create_standard_array(nLayers,(const unsigned int*) NeuronArray);
    cout << "Network was created.\n";
    
    // Define activation functions:
    // Avaliabale options:
    // FANN_LINEAR, FANN_THRESHOLD, FANN_THRESHOLD_SYMMETRIC, FANN_SIGMOID, FANN_SIGMOID_STEPWISE, FANN_SIGMOID_SYMMETRIC, FANN_SIGMOID_SYMMETRIC_STEPWISE, FANN_SIN, FANN_COS,
    // FANN_GAUSSIAN, FANN_GAUSSIAN_SYMMETRIC, FANN_ELLIOT, FANN_ELLIOT_SYMMETRIC, FANN_LINEAR_PIECE, FANN_LINEAR_PIECE_SYMMETRIC, FANN_SIN_SYMMETRIC, FANN_COS_SYMMETRIC

    // Loop over all layers & all neurons in the:
    for (Int_t kLayer = 1; kLayer<nLayers; ++kLayer)
    {
        // We skip kNeuron==0 because that is the input-layer.
        
        for (Int_t kNeuron = 0; kNeuron<(NeuronArray[kLayer]+1); ++kNeuron)
        {
            // We continue one neuron further, because those neurons are the bias-neurons.
            
            // Define activation function:
            fann_set_activation_function(ann,FANN_SIGMOID_SYMMETRIC,kLayer,kNeuron);
            
            // Define steepness of that function:
            fann_set_activation_steepness(ann,Activation_Steepness,kLayer,kNeuron);
        }
    }
    
    cout << "Activation functions were defined.\n";
    
    // Define learning options:
    fann_set_training_algorithm(ann,FANN_TRAIN_RPROP); // Options: FANN_TRAIN_RPROP & FANN_TRAIN_QUICKPROP
    fann_set_learning_rate(ann,LearningRate);
    //fann_set_learning_momentum(ann,0.8);
    //fann_set_rprop_increase_factor(ann,5);
    //fann_set_rprop_decrease_factor(ann,0.007);
    //fann_set_rprop_delta_max(ann,17);
    //fann_set_rprop_delta_zero(ann,0.01);
    cout << "Learning algorithms were specified.\n";
    
    // Save network:
    fann_save(ann,"./TheNetwork.net");
    fann_destroy(ann);
    cout << "Network was saved.\n";
    
    // Done.
}

// Define the main for stand-alone compilation:
#ifndef __CINT__
#include <TApplication.h>
int main(int argc, char** argv)
{
  TApplication* TheApp = new TApplication("Design_Network",&argc,argv);
  Design_Network();
  // TheApp->Run(); ==> This macro does not contain graphics.
  return 0;
}
#endif
