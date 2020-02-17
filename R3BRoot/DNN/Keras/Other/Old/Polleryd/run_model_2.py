
from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import tensorflow as tf
import neuland_dataset as nd
import time
import model_handler as mdh
# import model_handler_tot_eh as mdh
import datetime
import os

VALIDATE_ONLY = False  # If you only want to test model. Set MODEL_TEST_SET_SIZE to 0. DO NOT FORGET TO CHANGE TFRECORD FOLDER!

MAX_TRAIN_STEPS = 164000
BATCH_SIZE = 15
LEARNING_RATE_START = 1e-4  # Learning rate = LEARNING_RATE_START * DECAY_RATE^(step / NUM_EXAMPLES)
DECAY_RATE = 1

NORMALIZE = True
DATA_SUBSET = None  # [0, 100]  # set to None if whole set is wanted. [start_record, stop_record]

# Test small part of test set
SMALL_TEST_STEPS = 100  # 100  # set to MAX_TRAIN_STEPS to skip this test
SMALL_TEST_SET_SIZE = 200  # set to 0 for whole set

# Save summaries etc.
SAVE_STEPS = 10

# Test large test and train set
LARGE_TEST_TEST_STEPS = 2000  # set to MAX_TRAIN_STEPS to skip this test
LARGE_TEST_TRAIN_STEPS = MAX_TRAIN_STEPS  # MAX_TRAIN_STEPS  # set to MAX_TRAIN_STEPS to skip this test
LARGE_TEST_SET_SIZE = 10000  # set to 0 for whole set
LARGE_TRAIN_SET_SIZE = 0  # set to 0 for whole set

MODEL_TEST_SET_SIZE = 0  # set to 0 for whole set

TEST_BATCH_SIZE = 10  # This needs to be low (~same as BATCH_SIZE) for large networks. Splits testing into smaller batches.

NEUTRON_NUMBER = 1  # Needs only to be set when predicting momenta
NEUTRON_NUMBER_POSSIBILITIES = 8  # Total number of neutron number possibilities (including 0)


DIRECTORY = "logs/"
# DIRECTORY = "C:/Users/Markus/Desktop/Exjobb/logs/"
# DIRECTORY = "/net/data1/marpol/"

# MODEL_NAME = "smc_2_momentum_from_position_1n"
# MODEL_NAME = "total_e_and_h_reduced_high_n_events"
# MODEL_NAME = "smc_2_tot_e_and_h_minedep_not_reduced"
# MODEL_NAME = "smc_2_minedep_not_normalized"
MODEL_NAME = "smc_3_tot_e_and_h"
# MODEL_NAME = "total_e_and_h_minedep_not_reduced_128_2_b_200_2"

# Run on multiple machines?
MULTI_MACHINE = False

# Machines to run jobs on
MACHINES = ['None']

# Number of replicas for running multiple machines
REPLICAS_TO_AGGREGATE = 1
TOTAL_NUM_REPLICAS = 1

# Host that does testing on whole set
TESTER_HOST = 0

# END OF CHANGEABLE SECTION.
# ----------------------------------------
# VALUES BELOW SHOULD NOT BE CHANGED


FILE_DIRECTORY = DIRECTORY + MODEL_NAME + "/"

# Create directory if not exists
if not os.path.exists(FILE_DIRECTORY):
    os.makedirs(FILE_DIRECTORY)

RUN_INFO_FILE = FILE_DIRECTORY + 'run_info'
TEST_SUMMARIES_FILE = FILE_DIRECTORY + 'test_log'
TRAIN_SUMMARIES_FILE = FILE_DIRECTORY + 'train_log'
LARGE_TEST_SUMMARIES_FILE = FILE_DIRECTORY + 'test_log_large_set'
LARGE_TRAIN_SUMMARIES_FILE = FILE_DIRECTORY + 'train_log_large_set'
MODEL_TEST_SUMMARIES_FILE = FILE_DIRECTORY + 'model_test'
LARGE_TEST_NEUTRON_SEPARATION_MATRIX_FILE = FILE_DIRECTORY + 'neutron_separation_matrix_large_test_set'
LARGE_TRAIN_NEUTRON_SEPARATION_MATRIX_FILE = FILE_DIRECTORY + 'neutron_separation_matrix_large_train_set'
MODEL_SAVE_PATH = FILE_DIRECTORY + 'model'
SAVE_PATH_ARCHITECTURE_PARAMS = FILE_DIRECTORY + 'architecture_params'


def write_run_info_file(data_set):

    small_test_set_size = data_set.test.num_examples() if SMALL_TEST_SET_SIZE > data_set.test.num_examples() or SMALL_TEST_SET_SIZE == 0 else SMALL_TEST_SET_SIZE
    large_test_set_size = data_set.test.num_examples() if LARGE_TEST_SET_SIZE > data_set.test.num_examples() or LARGE_TEST_SET_SIZE == 0 else LARGE_TEST_SET_SIZE
    large_train_set_size = data_set.train.num_examples() if LARGE_TRAIN_SET_SIZE > data_set.train.num_examples() or LARGE_TRAIN_SET_SIZE == 0 else LARGE_TRAIN_SET_SIZE
    if DATA_SUBSET is None:
        data_subset = data_set.train.num_examples()
    else:
        data_subset = data_set.train.num_examples() if DATA_SUBSET[1] > data_set.train.num_examples() else DATA_SUBSET

    if LARGE_TRAIN_SET_SIZE > data_set.train.num_examples():
        large_train_set_size = data_set.train.num_examples()

    run_info_summary = '\n\nStart run: ' + datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S") + '\n' + \
                       'MAX_TRAIN_STEPS: ' + str(MAX_TRAIN_STEPS) + '\n' + \
                       'BATCH_SIZE: ' + str(BATCH_SIZE) + '\n' + \
                       'LEARNING_RATE_START: ' + str(LEARNING_RATE_START) + '\n' + \
                       'DECAY_RATE: ' + str(DECAY_RATE) + '\n' + \
                       'SMALL_TEST_STEPS: ' + str(SMALL_TEST_STEPS) + '\n' + \
                       'SMALL_TEST_SET_SIZE: ' + str(small_test_set_size) + '\n' + \
                       'SAVE_STEPS: ' + str(SAVE_STEPS) + '\n' + \
                       'LARGE_TEST_TEST_STEPS: ' + str(LARGE_TEST_TEST_STEPS) + '\n' + \
                       'LARGE_TEST_TRAIN_STEPS: ' + str(LARGE_TEST_TRAIN_STEPS) + '\n' + \
                       'LARGE_TEST_SET_SIZE: ' + str(large_test_set_size) + '\n' + \
                       'LARGE_TRAIN_SET_SIZE: ' + str(large_train_set_size) + '\n' + \
                       'REPLICAS_TO_AGGREGATE: ' + str(REPLICAS_TO_AGGREGATE) + '\n' + \
                       'TOTAL_NUM_REPLICAS: ' + str(TOTAL_NUM_REPLICAS) + '\n' + \
                       'NEUTRON_NUMBER (momenta prediction):' + str(NEUTRON_NUMBER) + '\n' + \
                       'NEUTRON_NUMBER_POSSIBILITIES:' + str(NEUTRON_NUMBER_POSSIBILITIES) + '\n' + \
                       'NORMALIZE: ' + str(NORMALIZE) + '\n' + \
                       'VALIDATE_ONLY: ' + str(VALIDATE_ONLY) + '\n' + \
                       'DATA_SUBSET: ' + str(data_subset) + '\n' + \
                       'MACHINES: '

    for machine in MACHINES:
        run_info_summary += machine + ", "

    write_summary(RUN_INFO_FILE, run_info_summary)


def write_summary(summary_file, summary):
    f = open(summary_file, 'a+')
    f.write(summary)
    f.close()


def print_matrix(matrix):
    matrix_print = ""
    for row in matrix:
        for col in row:
            matrix_print += str("%.2f" % round(col, 2)) + "\t"
        matrix_print += "\n"

    return matrix_print


def train_model():

    neuland = nd.NeulandDataset(flatten=False, start_stop_records=DATA_SUBSET, normalize=NORMALIZE)
    print('All files  loaded')

    # Write run-info file
    write_run_info_file(neuland)

    global_params = {'decay_steps': neuland.train.num_examples(),
                     'learning_rate_start': LEARNING_RATE_START,
                     'decay_rate': DECAY_RATE, 'replicas_to_aggregate': REPLICAS_TO_AGGREGATE,
                     'total_num_replicas': TOTAL_NUM_REPLICAS, 'neutron_number': NEUTRON_NUMBER,
                     'multi_machine': MULTI_MACHINE, 'neutron_number_possibilities': NEUTRON_NUMBER_POSSIBILITIES,
                     'max_train_steps': MAX_TRAIN_STEPS, 'save_steps': SAVE_STEPS,
                     'model_save_path': MODEL_SAVE_PATH, 'save_directory': FILE_DIRECTORY}

    with tf.device('/gpu:0'):
        model = mdh.ModelHandler(global_params=global_params)

    test_batch_size = TEST_BATCH_SIZE
    while model.training:
        start = time.time()  # int(round(time.time() * 1000))

        print("Step %d" % model.global_step)

        if VALIDATE_ONLY or MAX_TRAIN_STEPS - model.global_step == 1:
            print("Testing model on large test set")

            acc_test, sep_mat = model.test(neuland.test, MODEL_TEST_SET_SIZE, test_batch_size)
            sep_mat = print_matrix(sep_mat)
            write_summary(MODEL_TEST_SUMMARIES_FILE, "%s,%s\n" % (model.global_step, acc_test))
            write_summary(MODEL_TEST_SUMMARIES_FILE, "%s,\n%s\n\n" % (model.global_step, sep_mat))
            print("Accuracy on model test set at step %s: %s" % (model.global_step, acc_test))
            print(sep_mat)
            print("Test time: ", int(round((time.time() - start) * 1000)), " ms")
            exit()

        if model.global_step % SMALL_TEST_STEPS == 0 and model.global_step > 0:
            print("Testing model on small test set")

            acc_test, sep_mat = model.test(neuland.test, SMALL_TEST_SET_SIZE, test_batch_size)
            write_summary(TEST_SUMMARIES_FILE, "%s,%s\n" % (model.global_step, acc_test))
            print("Accuracy at step %s: %s" % (model.global_step, acc_test))
            print("Test time: ", int(round((time.time() - start) * 1000)), " ms")

        if model.global_step % LARGE_TEST_TEST_STEPS == 0 and model.global_step > 0:
            print("Testing model on large test set")

            acc_test, sep_mat = model.test(neuland.test, LARGE_TEST_SET_SIZE, test_batch_size)
            sep_mat = print_matrix(sep_mat)
            write_summary(LARGE_TEST_SUMMARIES_FILE, "%s,%s\n" % (model.global_step, acc_test))
            write_summary(LARGE_TEST_NEUTRON_SEPARATION_MATRIX_FILE, "%s,\n%s\n\n" % (model.global_step, sep_mat))
            print("Accuracy on large test set at step %s: %s" % (model.global_step, acc_test))
            print(sep_mat)
            print("Test time: ", int(round((time.time() - start) * 1000)), " ms")

        if model.global_step % LARGE_TEST_TRAIN_STEPS == 0 and model.global_step > 0:
            print("Testing model on large train set")

            acc_test, sep_mat = model.test(neuland.train, LARGE_TRAIN_SET_SIZE, test_batch_size)
            sep_mat = print_matrix(sep_mat)
            write_summary(LARGE_TRAIN_SUMMARIES_FILE, "%s,%s\n" % (model.global_step, acc_test))
            write_summary(LARGE_TRAIN_NEUTRON_SEPARATION_MATRIX_FILE, "%s,\n%s\n\n" % (model.global_step, sep_mat))
            print("Accuracy on large train set at step %s: %s" % (model.global_step, acc_test))
            print(sep_mat)
            print("Test time: ", int(round((time.time() - start) * 1000)), " ms")

        # Train model
        model.train_batch(neuland.train, BATCH_SIZE)
        print("Iteration time: ", int(round((time.time() - start) * 1000)), " ms")


train_model()


