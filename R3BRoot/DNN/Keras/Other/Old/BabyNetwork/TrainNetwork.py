# Load Keras package:
import keras as ks;

# Load numpy to handle matrices:
import numpy as np;

# Load rest of TensorFlow:
import tensorflow as tf;

# Define Network:
def model():
    
    # Define input layer with 2 neurons:
    InputLayer = ks.Input((2,))
    
    # Define hidden layer:
    MiddleLayer = ks.layers.Dense(6,activation='sigmoid',use_bias=True)(InputLayer)
    
    # Define output layer:
    OutputLayer = ks.layers.Dense(6,activation='sigmoid',use_bias=True)(MiddleLayer)
    
    # Now generate network architecture:
    TheModel = ks.models.Model(inputs=InputLayer,outputs=OutputLayer,name='BabyMultNetwork')
    
    # Return network as function output:
    return(TheModel)
    
    # Function definition ends when you go back with TAB:
#--------------------------------------------------------

# Define optimizer:
TheOptimizer = ks.optimizers.Adagrad(lr=0.3,epsilon=None,decay=0.0)

# Now compile model by calling it once:
TheModel=model()

# Compile model:
TheModel.compile(optimizer=TheOptimizer,loss='mean_squared_error',metrics=['accuracy'])

# Create training data:
import sys
sys.path.insert(0, '../../TrainingSets/Baby/')
import KERAS_Training as TD;

# Now we can train the network:
TheModel.fit(x=TD.InputData,y=TD.OutputData,epochs=1000,batch_size=1000000)

# Next, save the model:
TheModel.save("./BabyMultNetwork.h5")
