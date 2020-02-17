# Load Keras package:
import keras as ks;

# Load numpy to handle matrices:
import numpy as np;

# Load rest of TensorFlow:
import tensorflow as tf;

# Define Network:
def model():
    
    # Define input layer with 2 neurons:
    InputLayer = ks.Input((6002,))
    
    # Define hidden layer:
    MiddleLayer1 = ks.layers.Dense(9000,activation='relu',use_bias=True)(InputLayer)
    MiddleLayer2 = ks.layers.Dense(1200,activation='relu',use_bias=True)(MiddleLayer1)
    
    # Define output layer:
    OutputLayer = ks.layers.Dense(5,activation='softmax',use_bias=True)(MiddleLayer2)
    
    # Now generate network architecture:
    TheModel = ks.models.Model(inputs=InputLayer,outputs=OutputLayer,name='ElenaMultNetwork')
    
    # Return network as function output:
    return(TheModel)
    
    # Function definition ends when you go back with TAB:
#--------------------------------------------------------

# Define optimizer:
TheOptimizer = ks.optimizers.Adagrad(lr=0.01,epsilon=None,decay =0.0)

# Now compile model by calling it once:
TheModel=model()

# Compile model:
TheModel.compile(optimizer=TheOptimizer,loss='categorical_crossentropy',metrics=['accuracy'])

# Create training data:
import sys
sys.path.insert(0, '../../TrainingSets/')
#import Elena_Normalized_1e3events_600MeV as TD;
import MT_Set as TD;

# Now we can train the network:
TheModel.fit(x=TD.InputData,y=TD.OutputData,epochs=15,batch_size=100) # Elena used 100 events per batch.

# Next, save the model:
TheModel.save("./ElenaMultNetwork.h5")
