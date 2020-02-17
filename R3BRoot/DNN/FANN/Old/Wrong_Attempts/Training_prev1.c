// Include C++ headers:
#include <stdio.h>
#include <stdlib.h>

// Include FANN:
#include "fann.h"

// Main function definition:
int main()
{
    // Define options:
    int ReadDemo = 1;
    
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
    fann_set_activation_function_hidden(ann, FANN_SIGMOID_SYMMETRIC);
    fann_set_activation_function_output(ann, FANN_SIGMOID_SYMMETRIC);
    
    // Then, train on a file:
    fann_train_on_file(ann,"DNN_TrainingFile.txt",20,1,1e-6);
    // 200 ==> max. number of epochs.
    // 10: status report every 10 epochs.
    // 1e-6: desired error.
    
    // ----------------------------------------------------------------
    printf("Testing network.\n");
    fann_type *calc_out;
    
	struct fann_train_data* data = fann_read_train_from_file("DNN_VerificationFile.txt");

	// for(int i = 0; i < fann_length_train_data(data); i++)
    for(int i = 0; i < 20; i++)
	{
		fann_reset_MSE(ann);
		calc_out = fann_test(ann, data->input[i], data->output[i]);

		printf("XOR test (%f, %f) -> %f, should be %f, difference=%f\n",
			   data->input[i][0], data->input[i][1], calc_out[0], data->output[i][0],
			   (float) fann_abs(calc_out[0] - data->output[i][0]));
	}
    
    // ----------------------------------------------------------------
    // Define verification counters:
    int Multiplicity[10];
    for (int k = 0; k<10; ++k) {Multiplicity[k] = 0;}
    
    // Open the verification file:
    printf("\n\nReading verification file...\n");
    FILE* VeriFile;
    VeriFile = fopen("DNN_VerificationFile.txt","r");
    double ReadVariable;
    int Index;
    
    // Read the first 3 numbers:
    fscanf(VeriFile,"%lf",&ReadVariable);
    int NLines = (int) ReadVariable;
    printf("Number of events: %1d\n",NLines);
    
    fscanf(VeriFile,"%lf",&ReadVariable);
    int Ninputs = (int) ReadVariable;
    printf("Number of Inputs: %1d\n",Ninputs);
    
    fscanf(VeriFile,"%lf",&ReadVariable);
    int Noutputs = (int) ReadVariable;
    printf("Number of Outputs: %1d\n",Noutputs);
    
    printf("\n");
    
    // check that this file matches:
    if ((Ninputs==NeuronArray[0])&&(Noutputs==NeuronArray[nLayers-1]))
    {
        // Define input array:
        double Inputs[Ninputs];
        
        // Define output array:
        double Outputs[Noutputs];
        
        // loop over the lines:
        if (ReadDemo==1) {NLines = 10;}
        
        for (int line = 0; line<NLines; ++line)
        {
            // Then first read inputs:
            for (int k = 0; k<Ninputs; ++k)
            {
                fscanf(VeriFile,"%lf",&ReadVariable);
                Inputs[k] = ReadVariable;
                if (ReadDemo==1) {printf(" %0.4f |",Inputs[k]);}
            }
            if (ReadDemo==1) {printf("\n");}
            
            // Next, read outputs:
            for (int k = 0; k<Noutputs; ++k)
            {
                fscanf(VeriFile,"%lf",&ReadVariable);
                Outputs[k] = ReadVariable;
                if (ReadDemo==1) {printf(" %0.4f |",Outputs[k]);}
            }
            if (ReadDemo==1) {printf("\n");}
            
            // extract multiplcity:
            Index = (int) Outputs[0];
            
            // Next, compute output with the network:
            double* Outputs_network = fann_run(ann,Inputs);
            
            // And process output:
            for (int k = 0; k<Noutputs; ++k)
            {
                if (ReadDemo==1) {printf("FANN Output: %0.4f |",Outputs_network[k]);}
            }
            if (ReadDemo==1) {printf("\n");}
            
            // Compare:
            if ((Outputs[0]-Outputs_network[0]>-0.5)&&(Outputs[0]-Outputs_network[0]<0.5)&&(Index<10))
            {
                // Then, multiplcity is OK:
                Multiplicity[Index] = Multiplicity[Index] + 1;
            }
        }
    }
    
    // Give output:
    printf("Number of Successful 0n events: %1d\n",Multiplicity[0]);
    printf("Number of Successful 1n events: %1d\n",Multiplicity[1]);
    printf("Number of Successful 2n events: %1d\n",Multiplicity[2]);
    printf("Number of Successful 3n events: %1d\n",Multiplicity[3]);
    printf("Number of Successful 4n events: %1d\n",Multiplicity[4]);
    printf("Number of Successful 5n events: %1d\n",Multiplicity[5]);
    printf("Number of Successful 6n events: %1d\n",Multiplicity[6]);
    printf("Number of Successful 7n events: %1d\n",Multiplicity[7]);
    printf("Number of Successful 8n events: %1d\n",Multiplicity[8]);
    printf("Number of Successful 9n events: %1d\n",Multiplicity[9]);
    fclose(VeriFile);
    
    // end main function:
	return 0;
}
