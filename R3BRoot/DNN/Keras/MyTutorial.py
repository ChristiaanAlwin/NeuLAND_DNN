# Load Keras package:
import keras as ks;

# Load numpy to handle matrices:
import numpy as np;

# Load rest of TensorFlow:
import tensorflow as tf;

# Functions in keras have capitals, subsects have small.

# Define achitercture of the network.
# ()=0 inputs; outputs hoef je niet te specificeren in function declaration
# Die specificeer je met return op het einde.
def model():
    
    # Define network layer by layer.
    # 2 = 2 input neurons, 2 sets () ==> moet shape zijn ipv hard integer. & , as blank #input data.
    InputLayer = ks.Input((2,))
    
    # Batch normalization layer to scale inputs. Keras needs scaled inputs!
    BatchLayer = ks.layers.BatchNormalization()(InputLayer)
    # axis (input argument): specifies normalization direction (must be events!)
    
    # Define hidden layer:
    MiddleLayer = ks.layers.Dense(6,activation='sigmoid',use_bias=True)(BatchLayer)
    # Dense: fully connected layers & input = previous layer.
    
    # Define output layer:
    OutputLayer = ks.layers.Dense(6,activation='sigmoid',use_bias=True)(MiddleLayer) # NB: tussen 0 & 1, niet -1 en 1! Dus classificatie moet tussen 0 en 1!
    
    # Now generate network architecture:
    TheModel = ks.models.Model(inputs = InputLayer, outputs = OutputLayer, name='MultBabyNetwork')
    # Since each layer is connected to its precessor, only inputs & outputs need to be given.
    
    # Return network as function output:
    return(TheModel)
    
    # Function definition ends when you go back with TAB:

# Now compile model by calling it once:
TheModel=model()

# Define custom loss function:
def OwnLossFunction(ytrue,ypredict): 
    # Loss hangt af van output neurons: ytrue & ypredict zijn hier 6x1 matrices.
    loss=tf.reduce_sum(tf.math.pow(tf.math.abs(ytrue-ypredict),0.2))
    return loss

# Compile model:
# TheModel.compile(optimizer='sgd',loss='mean_squared_error') # Default loss function
TheModel.compile(optimizer='sgd',loss=OwnLossFunction)
# Optimizer: learning algorithm: ='sgd' (stochastic gradient decent). or ='Adam' for ADAM.
# loss: the function that has to be optimized by adjusting network weights. We can
# manually define it, but a default can be done too.

# To train the network:
# Command is: TheModel.fit(x=,y=,epochs=,batch_size=)
# x = input-data matrix, y=output-data matrix, epochs= aantal keren dat je over de HELE dataset looped.
# batch_size aantal evenst binnen de dataset per iteratie (dus 1 epoch = #batch_size iteraties).

# Define input-data matrix (2=#input neurons, 10=#training events) ==> 2x10 matrix.
# InputData[0,0] = number specifies a single entry and we start counting from 0 here!
# :notation works almost like Matlab. ,=column-matrix & ;=row-matrix.
InputData = np.ones((2,10)) # shape specificeren is altijd dubbele haken.
InputData[:,0] = [2.400e1,1.406e3]
InputData[:,1] = [3.000e0,3.314e2]
InputData[:,2] = [9.000e0,4.489e2]
InputData[:,3] = [1.100e1,5.538e2]
InputData[:,4] = [1.400e1,1.073e3]
InputData[:,5] = [7.000e0,2.780e2]
InputData[:,6] = [1.000e1,4.909e2]
InputData[:,7] = [5.000e0,1.908e2]
InputData[:,8] = [6.000e0,5.749e2]
InputData[:,9] = [4.000e0,2.753e2]

# Define output data:
OutputData = np.ones((6,10))
OutputData[:,0] = [-1.000e0,-1.000e0,-1.000e0,-1.000e0,-1.000e0,1.000e0]
OutputData[:,1] = [-1.000e0,1.000e0,-1.000e0,-1.000e0,-1.000e0,-1.000e0]
OutputData[:,2] = [-1.000e0,-1.000e0,1.000e0,-1.000e0,-1.000e0,-1.000e0]
OutputData[:,3] = [-1.000e0,-1.000e0,1.000e0,-1.000e0,-1.000e0,-1.000e0]
OutputData[:,4] = [-1.000e0,-1.000e0,-1.000e0,-1.000e0,1.000e0,-1.000e0]
OutputData[:,5] = [-1.000e0,1.000e0,-1.000e0,-1.000e0,-1.000e0,-1.000e0] 
OutputData[:,6] = [-1.000e0,-1.000e0,-1.000e0,1.000e0,-1.000e0,-1.000e0] 
OutputData[:,7] = [-1.000e0,1.000e0,-1.000e0,-1.000e0,-1.000e0,-1.000e0] 
OutputData[:,8] = [-1.000e0,-1.000e0,1.000e0,-1.000e0,-1.000e0,-1.000e0] 
OutputData[:,9] = [-1.000e0,1.000e0,-1.000e0,-1.000e0,-1.000e0,-1.000e0] 

# Then transpose matrix, because keras wants that:
InputData = InputData.T
OutputData = OutputData.T 

# Now we can train the network:
TheModel.fit(x=InputData,y=OutputData,epochs=1000,batch_size=3)

# Next, test the model:
InputTest = np.ones((2,6))
InputTest[:,0] = [2.100e1,1.014e3]
InputTest[:,1] = [2.500e1,1.371e3]
InputTest[:,2] = [1.400e1,1.014e3]
InputTest[:,3] = [1.500e1,6.664e2]
InputTest[:,4] = [1.400e1,1.073e3] # training data
InputTest[:,5] = [7.000e0,2.780e2] # training data
InputTest = InputTest.T
print(InputTest)

OutputTest = np.ones((6,6))
OutputTest[:,0] = [-1.000e0,-1.000e0,-1.000e0,-1.000e0,-1.000e0,1.000e0]
OutputTest[:,1] = [-1.000e0,-1.000e0,-1.000e0,-1.000e0,-1.000e0,1.000e0]
OutputTest[:,2] = [-1.000e0,-1.000e0,-1.000e0,-1.000e0,1.000e0,-1.000e0] 
OutputTest[:,3] = [-1.000e0,-1.000e0,1.000e0,-1.000e0,-1.000e0,-1.000e0] 
OutputTest[:,4] = [-1.000e0,-1.000e0,-1.000e0,-1.000e0,1.000e0,-1.000e0] # training data
OutputTest[:,5] = [-1.000e0,1.000e0,-1.000e0,-1.000e0,-1.000e0,-1.000e0] # training data
OutputTest = OutputTest.T

# Make predictions:
Prediction = TheModel.predict(InputTest)

# Print output:
print(Prediction)
print(OutputTest)







# Load_weights-function is to load a pre-trained network.
