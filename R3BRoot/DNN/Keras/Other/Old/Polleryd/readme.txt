
In order to run the scripts you need these files:

model_handler.py
models_2.py
run_model_2.py
neuland_dataset.py
neuland_readfile_dataset.py
convert_to_tfrecord.py
placeholder_functions.py


A text file with the neutron momenta of each event structured as:

Event	0	Guns	2
402.161	-20.685343	-19.883186	957.403809

797.448	-103.000572	49.198177	1456.503540
Event	1	Guns	2

797.448	-103.000572	49.198177	1456.503540

734.255	12.625445	86.885208	1382.455078
Event	2	Guns	2
734.255	12.625445	86.885208	1382.455078
402.161	-20.685343	-19.883186	957.403809

...
...
...


A text file with the hits of each event structured as:





1. convert_to_tfrecord.py

Should be run on its own. It writes the data in the gun file and the hit file to a TF record file.
Removes hits with NaN values and time values larger than 200. At the bottom of this file you need
to specify which text files to read from.

The main function is "write_train_test_validation_files":

# Writes simulation data to TFRecord file and splits into train, test and validation files
# If full_image is true the output will be a 3d image with time and energy values in each "pixel".
# gun_file and hit_file should be lists of files from simulations. They need to be in the same
# order, meaning that gun_file[i] has to be from the same simulation as hit_file[i]. The maximum
# amount of events is determined from the file with fewest events.
# ttv_distribution should be a list of 4 numbers with starting event for each train, test and
# validation file. (0,1000,2000,3000) will give the train data the first 1000 events, the test
# data the events between 1000 and 2000 etc.
	


2. neuland_dataset.py & neuland_readfile_dataset.py

"neuland_dataset.py" creates an object containing the data from the TF record files created by 
"convert_to_tfrecord". It takes three arguments: (flatten, start_stop_records, normalize).
If flatten is true the events will flattened into a 1D array. 
start_stop_records should be an array with two values giving the subset of the events wanted from
the dataset. e.g. [0,100] will give the first 100 events of the dataset. "None" will give the whole
set.
If normalize is true, the data is normalized according to the functions in the constructor (__init__).

The most useful function is the "next_batch" which gives a batch of events from the dataset for training.
It shuffles all events (if set to true) and once all events in the dataset have been used, it shuffles
them again.

In this file you need to set the path to the TF record files you want to use.

"neuland_readfile_dataset.py" is used by "neuland_dataset.py" to read the tfrecord files. 



3. models_2.py & placeholder_functions.py

Uses functions from "placeholder_functions.py" and defines the architecture of the different models and
all parameter/weight initializations.



4. model_handler.py

This is where you set the model from "models_2.py", cost function and optimization algorithm you want
to use. It contains the functions "train_batch" and "test" that should be used for training and
testing. "train_batch" takes data_set as argument, which is should be the "neuland_dataset._train" object.
It also takes batch_size as argument.
"test" also takes data_set as argument which should be the "neuland_dataset._test" object. test_set_size
determines how many events of the set that should be tested. batch_size is the number of images that are
sampled in each test. Setting a low number reduces the memory consumption which is useful when dealing
with large networks.



5. run_model_2.py

This is the main file that uses all the files above except "convert_to_tfrecord.py". Here you have many
parameters that can be fine tuned such as batch size, maximum training steps, at what interval the model
is saved etc.




How to run:

- Run "convert_to_tfrecord.py" to get the tfrecord files. 
- Make all desired changes in models_2.py and model_handler.py to create the architecture and training
	procedure your model.
- Make all desired changes in run_model_2.py and run it!


