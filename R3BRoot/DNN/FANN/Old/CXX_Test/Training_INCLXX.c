// Include C++ headers:
#include <stdio.h>
#include <stdlib.h>

// Include FANN:
#include "fann.h"

// Main function definition:
int main()
{
    // Set options:
    int UseMaximumOutput = 1;  // 0=kFALSE, 1=kTRUE
    int LimitToTrueEvents = 1; // 0=kFALSE, 1=kTRUE
    
    // ----------------------------------------------------------------
    
    // Design the network:
    printf("\n\nDesign the network:\n");

    // Define network configuration:
    unsigned int nLayers = 3;
    const unsigned int NeuronArray[3] = {2,6,6};
    // This newtork has 3 layers in total, including
    // input, hidden & output layers. The array of int gives
    // the number of neurons in each layer. A fully
    // connected network is made.
    
    // Next, create the network:
    struct fann* ann = fann_create_standard_array(nLayers,NeuronArray);
    
    // Define activation function:
    // first input: the network.
    // second input: the activation function. Options: FANN_LINEAR, FANN_THRESHOLD, FANN_THRESHOLD_SYMMETRIC, FANN_SIGMOID
    //                                                 FANN_SIGMOID_STEPWISE, FANN_SIGMOID_SYMMETRIC, FANN_SIGMOID_SYMMETRIC_STEPWISE,
    //                                                 FANN_GAUSSIAN, FANN_GAUSSIAN_SYMMETRIC, FANN_ELLIOT, FANN_ELLIOT_SYMMETRIC,
    //                                                 FANN_LINEAR_PIECE, FANN_LINEAR_PIECE_SYMMETRIC, FANN_SIN_SYMMETRIC, FANN_COS_SYMMETRIC
    //                                                 FANN_SIN, FANN_COS.
    // Third input: the layer (0=input layer). Fourth input: the neuron inside that layer.
    for (int kLayer = 1; kLayer<nLayers; ++kLayer)
    {
        for (int kNeuron = 0; kNeuron<(NeuronArray[kLayer]+1); ++kNeuron) // +1=bias neuron.
        {
            fann_set_activation_function(ann,FANN_SIGMOID_SYMMETRIC,kLayer,kNeuron);
        }
    }
    
    // Set steepness of activation function. Same structure as before, but second input is steepness in [0,1] interval:
    for (int kLayer = 1; kLayer<nLayers; ++kLayer)
    {
        for (int kNeuron = 0; kNeuron<(NeuronArray[kLayer]+1); ++kNeuron)
        {
            fann_set_activation_steepness(ann,1.0,kLayer,kNeuron);
        }
    }
    
    // ----------------------------------------------------------------
    
    // Train the network:
    printf("\n\nTrain the network:\n");
    
    // Select the training algorithm:
    // Options: FANN_TRAIN_RPROP, FANN_TRAIN_QUICKPROP
    fann_set_training_algorithm(ann,FANN_TRAIN_RPROP);
    
    // Specify learning parameters:
    // fann_set_learning_rate(ann,0.01);
    // fann_set_learning_momentum(ann,0.8);
    //fann_set_rprop_increase_factor(ann,5);
    //fann_set_rprop_decrease_factor(ann,0.007);
    //fann_set_rprop_delta_max(ann,17);
    //fann_set_rprop_delta_zero(ann,0.01);
    
    // fann_train_on_file(ann,"DNN_TestFile1.txt",50,1,1e-6);
    fann_train_on_file(ann,"./Data_PhysList/INCLXX_Training.txt",2000,10,1e-6);
    // 1) the network.
    // 2) string describing the txt-file containing the data.
    // 3) max. number of epochs.
    // 4) desired error for fitting.
    
    // Save the network:
    fann_save(ann,"./DNN_Float.net");
    
    // end main function:
	return 0;
}
