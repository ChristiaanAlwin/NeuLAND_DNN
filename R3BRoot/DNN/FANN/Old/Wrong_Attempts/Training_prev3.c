// Include C++ headers:
#include <stdio.h>
#include <stdlib.h>

// Include FANN:
#include "fann.h"

// Main function definition:
int main()
{
    // ----------------------------------------------------------------
    
    // Design the network:
    printf("\n\nDesign the network:\n");

    // Define network configuration:
    unsigned int nLayers = 3;
    const unsigned int NeuronArray[3] = {2,2,1};
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
        for (int kNeuron = 0; kNeuron<NeuronArray[kLayer]; ++kNeuron)
        {
            fann_set_activation_function(ann,FANN_SIGMOID,kLayer,kNeuron);
        }
    }
    
    // Set steepness of activation function. Same structure as before, but second input is steepness in [0,1] interval:
    for (int kLayer = 1; kLayer<nLayers; ++kLayer)
    {
        for (int kNeuron = 0; kNeuron<NeuronArray[kLayer]; ++kNeuron)
        {
            fann_set_activation_steepness(ann,0.5,kLayer,kNeuron);
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
    //fann_set_learning_momentum(ann,0.7);
    //fann_set_rprop_increase_factor(ann,5);
    //fann_set_rprop_decrease_factor(ann,0.007);
    //fann_set_rprop_delta_max(ann,17);
    //fann_set_rprop_delta_zero(ann,0.01);
    
    fann_train_on_file(ann,"DNN_TestFile1.txt",50,1,1e-6);
    // fann_train_on_file(ann,"DNN_TrainingFile.txt",3,1,1e-6);
    // 1) the network.
    // 2) string describing the txt-file containing the data.
    // 3) max. number of epochs.
    // 4) desired error for fitting.
    
    // ----------------------------------------------------------------
    
    // Test the network:
    printf("\n\nTest the network:\n");
    
    // This, we will need to obtain the output:
    fann_type *calc_out;
    
    // Declare what me need to verify the multiplcity:
    double Output;
    int const MultSize = 10;
    int Calculated_Multiplicity[MultSize];
    int DataSet_Multiplicity[MultSize];
    int Index;
    for (int k = 0; k<MultSize; ++k) 
    {
        Calculated_Multiplicity[k] = 0;
        DataSet_Multiplicity[k] = 0;
    }
    
    // connect to a verification file:
	// struct fann_train_data* data = fann_read_train_from_file("DNN_VerificationFile.txt");
	struct fann_train_data* data = fann_read_train_from_file("DNN_TestFile1.txt");

    // Next, do a for-loop over this data:
	for(int i = 0; i < fann_length_train_data(data); i++)
	{
        // test a single output:
		calc_out = fann_test(ann, data->input[i], data->output[i]);

        // Give some output:
        if (i<20)
        {
            printf("Test (%f, %f) -> %f, should be %f, difference=%f\n",data->input[i][0], data->input[i][1], calc_out[0], data->output[i][0],(float) fann_abs(calc_out[0] - data->output[i][0]));
        }
        
        // Count computed multiplicity:
        Output = calc_out[0];
        Index = ((int) (Output+0.5));
        if ((Index>=0)&&(Index<10)) {Calculated_Multiplicity[Index] = Calculated_Multiplicity[Index] + 1;}
        
        // Count true multiplicity:
        Output = data->output[i][0];
        Index = ((int) (Output+0.5));
        if ((Index>=0)&&(Index<10)) {DataSet_Multiplicity[Index] = DataSet_Multiplicity[Index] + 1;}
	}
    
    // ----------------------------------------------------------------
    
    // Give output:
    printf("\n\nGive final output:\n");
    
    // Print multiplcity results:
    for (int k = 0; k<MultSize; ++k)
    {
        printf("Number of %1d neutron events: %1d [Network] | %1d [Dataset]\n",k,Calculated_Multiplicity[k],DataSet_Multiplicity[k]);
    }
    
    // print network parameters:
    printf("\n\n");
    // fann_print_parameters(ann);
    
    // Print network weights:
    unsigned int connum = fann_get_total_connections(ann);
    struct fann_connection *con;
    con = calloc(connum, sizeof(*con));
    fann_get_connection_array(ann,con);
    
    for (int i = 0; i < connum; ++i) {
        printf("Weight of Connection: from neuron %1d --> to neuron %1d | Value = %1f\n", con[i].from_neuron,con[i].to_neuron,con[i].weight);
    }
    
    free(con);
    
    // end main function:
	return 0;
}
