# NeuLAND_DNN
Deep Neural Network for NeuLAND Data Analysis, an ENSAR2 project by C. A. Douma

This is a version of R3BRoot (https://github.com/R3BRootGroup/R3BRoot.git) that contains
a DNN (Deep Neural Network) module for the NeuLAND Data Analysis. 

First, one must install FairSoft and FairRoot in the usual way.
The .tar-version supplied in this repository are guaranteed to be
compatible with this version of R3BRoot. Then, this version of R3BRoot
should be installed on top of it.

Next, one should install Keras and TensorFlow software in order to
be able to use this package. The software package is controlled by
the Train, Validate & Experiment scripts in the DNN folder. There,
a README file is also available for how to use these scripts and
for how Keas and TensorFlow should be installed and linked to the 
DNN control scripts.
