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
    
    // Next, retrieve the network from a file:
    struct fann* ann = fann_create_from_file("./DNN_Float.net");
    
    // ----------------------------------------------------------------
    
    // Test the network:
    printf("\n\nTest the network:\n");
    
    // This, we will need to obtain the output:
    fann_type *calc_out;
    
    // Declare what me need to verify the multiplcity:
    double Output;
    double Maximum;
    int const MultSize = 10;
    int Calculated_Multiplicity[MultSize];
    int DataSet_Multiplicity[MultSize];
    int Index;
    int TrueMultIndex;
    for (int k = 0; k<MultSize; ++k) 
    {
        Calculated_Multiplicity[k] = 0;
        DataSet_Multiplicity[k] = 0;
    }
    
    // connect to a verification file:
	struct fann_train_data* data = fann_read_train_from_file("./Data_PhysList/BERT_Validation.txt");
	// struct fann_train_data* data = fann_read_train_from_file("DNN_TestFile1.txt");

    // Next, do a for-loop over this data:
	for(int i = 0; i < fann_length_train_data(data); i++)
	{
        // test a single output:
		calc_out = fann_test(ann, data->input[i], data->output[i]);

        // Give some output:
        if (i<20)
        {
            printf("Test (%f, %f) -> (%f, %f, %f, %f, %f, %f), should be (%f, %f, %f, %f, %f, %f)\n",
                   data->input[i][0], data->input[i][1], 
                   calc_out[0], calc_out[1], calc_out[2], calc_out[3], calc_out[4], calc_out[5], 
                   data->output[i][0], data->output[i][1], data->output[i][2], data->output[i][3], data->output[i][4], data->output[i][5]);
        }
        
        // Count true multiplicity:
        if (UseMaximumOutput==1)
        {
            Index = -1;
            TrueMultIndex = -1;
            Maximum = -1e99;
            
            for (int k = 0; k<NeuronArray[nLayers-1]; ++k)
            {
                if (Maximum<data->output[i][k])
                {
                    Maximum = data->output[i][k];
                    Index = k;
                }
            }
        }
        else
        {
            Index = -1;
            for (int k = 0; k<NeuronArray[nLayers-1]; ++k)
            {
                for (int n = 0; n<NeuronArray[nLayers-1]; ++n)
                {
                    if (k!=n)
                    {
                        if ((data->output[i][k]>0.0)&&(data->output[i][n]<0.0))
                        {
                            Index = k;
                        }
                    }
                }
            }
        }
        if (Index>=0) {DataSet_Multiplicity[Index] = DataSet_Multiplicity[Index] + 1; TrueMultIndex = Index;}
        
        // Count computed multiplicity:
        if (UseMaximumOutput==1)
        {
            Index = -1;
            Maximum = -1e99;
            
            for (int k = 0; k<NeuronArray[nLayers-1]; ++k)
            {
                if (Maximum<calc_out[k])
                {
                    Maximum = calc_out[k];
                    Index = k;
                }
            }
        }
        else
        {
            Index = -1;
            for (int k = 0; k<NeuronArray[nLayers-1]; ++k)
            {
                for (int n = 0; n<NeuronArray[nLayers-1]; ++n)
                {
                    if (k!=n)
                    {
                        if ((calc_out[k]>0.0)&&(calc_out[n]<0.0))
                        {
                            Index = k;
                        }
                    }
                }
            }
        }
        
        if (Index>=0) 
        {
            if (LimitToTrueEvents==1)
            {
                if ((Index==TrueMultIndex)&&(TrueMultIndex>=0))
                {
                    Calculated_Multiplicity[Index] = Calculated_Multiplicity[Index] + 1;
                }
            }
            else
            {
                Calculated_Multiplicity[Index] = Calculated_Multiplicity[Index] + 1;
            }
        }
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
    /*
    // Print network weights:
    unsigned int connum = fann_get_total_connections(ann);
    struct fann_connection *con;
    con = calloc(connum, sizeof(*con));
    fann_get_connection_array(ann,con);
    
    for (int i = 0; i < connum; ++i) {
        // printf("Weight of Connection: from neuron %1d --> to neuron %1d | Value = %1f\n", con[i].from_neuron,con[i].to_neuron,con[i].weight);
    }
    
    free(con);
    */
    // end main function:
	return 0;
}
