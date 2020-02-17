# Load Keras package:
import keras as ks;

# Load numpy to handle matrices:
import numpy as np;

# Load rest of TensorFlow:
import tensorflow as tf;

# Load (& auto-compile) teh model"
TheModel=ks.models.load_model("./ElenaMultNetwork.h5")

# Next, import the validation data:
#import sys
#sys.path.insert(0, './')
#import TrainingSet_Norm as VD;

import sys
sys.path.insert(0, '../../TrainingSets/')
#import Elena_Normalized_1e3events_600MeV as VD;
import MT_Set as VD;

# Make the model predictions:
Prediction = TheModel.predict(VD.InputData)

# Our next task is to extract the multiplicity efficiency.
# Hence, we need to compare the data to the true outputs.

# Extract the size parameters:
nEvents = Prediction.shape[0]  # Size (#rows) of matrix.
nOutputs = Prediction.shape[1] # Size (#columns) of matrix

# Define counters:
Predicted_Multiplicities = np.zeros((nOutputs,1))
True_Multiplicities = np.zeros((nOutputs,1))
Predicted_Limited_Multiplicities = np.zeros((nOutputs,1))

# Define maximum search parameters:
Maximum_Predicted = 0.0
MaxIndex_Predicted = -1
Maximum_True = 0.0
MaxIndex_True = -1

# Loop over events:
for n in range(0,nEvents): # doet 0 t/m nEvents-1
    
    # Find out maximum & position in prediction:
    MaxIndex_Predicted = -1
    Maximum_Predicted = -100.0
    
    for k in range(0,nOutputs):
        if Maximum_Predicted<Prediction[n,k]:
            Maximum_Predicted = Prediction[n,k]
            MaxIndex_Predicted = k
            
    # Find out maximum in the true outputs:
    Maximum_True = -100.0
    MaxIndex_True = -1
    
    for k in range(0,nOutputs):
        if Maximum_True<VD.OutputData[n,k]:
            Maximum_True = VD.OutputData[n,k]
            MaxIndex_True = k
            
    # Next, update the counters:
    Predicted_Multiplicities[MaxIndex_Predicted,0] = Predicted_Multiplicities[MaxIndex_Predicted,0] + 1
    True_Multiplicities[MaxIndex_True,0] = True_Multiplicities[MaxIndex_True,0] + 1
    
    if MaxIndex_Predicted==MaxIndex_True:
        Predicted_Limited_Multiplicities[MaxIndex_True,0] = Predicted_Limited_Multiplicities[MaxIndex_True,0] + 1
    
    # Close all blocks:

# Next, to get the percentages, do a division:
Percentages = np.zeros((nOutputs,1))

for k in range(0,nOutputs):
    Percentages[k,0] = Predicted_Limited_Multiplicities[k,0]/True_Multiplicities[k,0]

# Then, print results:
print('Multiplicity efficiencies are:')
print(Percentages)

print('')
print(Predicted_Limited_Multiplicities)
print(Predicted_Multiplicities)
print(True_Multiplicities)
