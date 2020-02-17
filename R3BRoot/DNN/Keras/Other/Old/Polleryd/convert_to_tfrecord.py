"""Converts text data to TFRecord file"""
from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import numpy as np
import tensorflow as tf

FLAGS = None
C_IN_PADDLE = 29.9792569/1.58  # cm/ns
TIME_RESOLUTION = 1.0  # ns


def _int64_feature(value):
    return tf.train.Feature(int64_list=tf.train.Int64List(value=[value]))


def _float_feature(value):
    return tf.train.Feature(float_list=tf.train.FloatList(value=[value]))


def _bytes_feature(value):
    return tf.train.Feature(bytes_list=tf.train.BytesList(value=[value]))


def _2dim_np_array_to_string(array):
    return '\n'.join('\t'.join('%0.3f' % x for x in y) for y in array)


# Extracts plane, paddle, t1, t2, e1, e2
# from file line
def parse_file_line_hits(line):

    # line_split = line.split("/")
    # line_split0 = line_split[0].split(' ')
    # line_split1 = line_split[1].split(' ')
    # line_split0 = list(filter(None, line_split0))
    # line_split1 = list(filter(None, line_split1))

    # values = [float(line_split0[0]), float(line_split1[0]),
    #          float(line_split1[2]), float(line_split1[4]),
    #          float(line_split1[6]), float(line_split1[8])]

    line = line.replace('/', ' ').replace('t1=', ' ').replace('t2=', ' ').replace('e1=', ' ').replace('e2=', ' ')
    line_split = line.split(' ')
    line_split = list(filter(None, line_split))

    values = [float(line_split[0].strip()), float(line_split[1].strip()), float(line_split[2].strip()),
              float(line_split[3].strip()), float(line_split[4].strip()), float(line_split[5].strip())]

    # skip if some number is not a number, ehh... or if time values are too large
    if np.isnan(values[0]) or np.isnan(values[1]) or np.isnan(values[2]) or np.isnan(values[3]) or np.isnan(values[4]):
        values = False
    elif values[2] > 200 or values[3] > 200:
        values = False

    return values


# Extracts e, px, py, pz and converts to float
def parse_file_line_guns(line):
    line_split = line.split('\t')
    return [float(line_split[0]), float(line_split[1]), float(line_split[2]), float(line_split[3])]


# Takes input values: plane, paddle, t1, t2, e1, e2
# and converts to 5 dim coordinate array [x,y,z,t,e]
def convert_to_coordinates(hits):

    e = np.sqrt(hits[4] * hits[5])  # e1 * e2
    t = (hits[3] + hits[2]) / 2.0  # (t2 + t1)/2
    z = 5 * hits[0]

    # pos_e = np.log(hits[5]) - np.log(hits[4]) # log(e2) - log(e1)
    pos_t = C_IN_PADDLE*(hits[3] - hits[2])/2  # c*(t2 - t1)/2

    # if plane number is even
    if hits[0] % 2 == 0:
        y = 5 * (hits[1] - 25.5)
        x = pos_t
    # if plane number is odd
    else:
        x = 5 * (hits[1] - 25.5)
        y = pos_t

    return [x, y, z, t, e]


# Takes input values: x, y, z, t, e and converts into integer grid
# values. x,y,z -> [0,49(z:59)], t -> t (no change) or [0, t_max/TIME_RESOLUTION],
# e -> e (no change)
def coordinates_to_grid(coordinates):
    x = 25 + np.floor(coordinates[0] / 5)
    y = 25 + np.floor(coordinates[1] / 5)
    z = np.floor(coordinates[2] / 5)
    # t = np.floor(coordinates[3] / TIME_RESOLUTION),
    t = coordinates[3] / TIME_RESOLUTION

    if x<0:
        x=0.0
    elif x>49:
        x=49.0
    if y<0:
        y=0.0
    elif y>49:
        y=49.0
    if z<0:
        z=0.0
    elif z>59:
        z=59.0

    return [x, y, z, t, coordinates[4]]


# reads all hits in event from file. Returns array of hits where each hit
# is given by [x, y, z, t, e]
def read_event_hit_file(file):
    line = file.readline()
    # If end of file return false
    if line == '':
        return False

    hits = []
    while True:
        # break if new event or end of file reached
        if line == '' or line[0] == 'E':
            break
        line = parse_file_line_hits(line)
        # skip hit if it contains 'nan' or large time values
        if line is False:
            line = file.readline()
            continue
        line = convert_to_coordinates(line)
        line = coordinates_to_grid(line)
        hits.append(line)
        line = file.readline()

    return hits


# reads all guns in event from file. Returns list of guns where each gun
# is given by [e, px, py, pz]
def read_event_gun_file(file):
    line = file.readline()
    # If end of file return false
    if line == '':
        return False

    guns = []
    #guns = np.zeros((6, 4))
    #i = 0
    while True:
        # break if new event or end of file reached
        if line == '' or line[0] == 'E':
            break
        guns.append(np.array(parse_file_line_guns(line)))
        #guns[i] = np.array(parse_file_line_guns(line))
        #i += 1
        line = file.readline()

    return guns


def event_hits_to_image(hits):
    # create 4d image [x:(0-49), y:(0-49), z:(0-59), t|e]
    event_image = np.zeros((50, 50, 60, 2))

    for hit in hits:
        # skip if some number is not a number, ehh...
        if np.isnan(hit[0]) or np.isnan(hit[1]) or np.isnan(hit[2]) or np.isnan(hit[3]) or np.isnan(hit[4]):
            continue
        event_image[int(hit[0])][int(hit[1])][int(hit[2])][0] = hit[3]
        event_image[int(hit[0])][int(hit[1])][int(hit[2])][1] = hit[4]

    return event_image


# Writes one event to a TFRecord file
# event_guns and event_hits should be of the same type as the output given by read_event_gun_file
# and read_event_hit_file. The writer should be a TFRecord writer.
def write_event(event_guns, event_hits, writer):
    event_guns_raw = np.array(event_guns).tostring()
    event_hits_raw = np.array(event_hits).tostring()

    gun_one_hot = np.zeros(8)  # 8 neutron possibilities (including zero)
    gun_one_hot[len(event_guns)] = 1
    gun_one_hot = gun_one_hot.tostring()

    # Sum up total energy
    total_e = 0
    if len(event_hits) > 0:
        total_e = sum_array(np.array(event_hits)[:, 4])

    example = tf.train.Example(features=tf.train.Features(feature={
        'total_e': _float_feature(total_e),
        'guns': _int64_feature(len(event_guns)),
        'hits': _int64_feature(len(event_hits)),
        'gun_one_hot': _bytes_feature(gun_one_hot),
        'guns_raw': _bytes_feature(event_guns_raw),
        'hits_raw': _bytes_feature(event_hits_raw)}))
    writer.write(example.SerializeToString())


def is_any_list_object_false(obj_list):
    for item in obj_list:
        if item is False:
            return True
    return False


# Sums all elements of array and discards the ones that are 'nan'
def sum_array(array):
    arr_sum = 0.
    for arr in array:
        if not np.isnan(arr):
            arr_sum += arr
    return arr_sum


# Writes simulation data to TFRecord file and splits into train, test and validation files
# If full_image is true the output will be a 3d image with time and energy values in each "pixel"
# gun_file and hit_file should be lists of files from simulations. They need to be in the same
# order, meaning that gun_file[i] has to be from the same simulation as hit_file[i]. The maximum
# amount of events is determined from the file with fewest events.
# ttv_distribution should be a list of 4 numbers with starting event for each train, test and
# validation file. (0,1000,2000,3000) will give the train data the first 1000 events, the test
# data the events between 1000 and 2000 etc.
def write_train_test_validation_files(full_image, gun_file, hit_file, dest_file_path, ttv_distribution):

    # skip a few rows in files
    for i in range(len(gun_file)):
        gun_file[i].readline()  # next(gun_file[i])
        hit_file[i].readline()  # next(hit_file[i])
        hit_file[i].readline()  # next(hit_file[i])
        hit_file[i].readline()  # next(hit_file[i])

    # Writers for train, test and validation data
    writer = [None] * 3
    writer[0] = tf.python_io.TFRecordWriter(dest_file_path + "train.tfrecords")
    writer[1] = tf.python_io.TFRecordWriter(dest_file_path + "test.tfrecords")
    writer[2] = tf.python_io.TFRecordWriter(dest_file_path + "validation.tfrecords")

    for j in range(3):
        start = ttv_distribution[j]
        stop = ttv_distribution[j+1]

        for ttv_set in range(start, stop):
            print(ttv_set)
            event_hits = [None] * len(gun_file)
            event_guns = [None] * len(gun_file)
            for i in range(len(gun_file)):
                event_hits[i] = read_event_hit_file(hit_file[i])
                event_guns[i] = read_event_gun_file(gun_file[i])

            # If end of file reached, break loop
            if is_any_list_object_false(event_guns) or is_any_list_object_false(event_hits):
                break

            if full_image:
                for i in range(len(gun_file)):
                    event_hits[i] = event_hits_to_image(event_hits[i])

            # Reduce the number of events with 6 or 7 neutrons
            neutrons = 1
            #if ttv_set % 2 == 0:
            #    neutrons = 7
            #if ttv_set % 4 == 0:
            #    neutrons = 8

            # set max number of neutrons in test and validation set
            #if j > 0:
            #    neutrons = 6  # 5 neutrons (including 0)

            for i in range(neutrons):
                write_event(event_guns[i], event_hits[i], writer[j])

    # Close all file streams
    writer[0].close()
    writer[1].close()
    writer[2].close()
    for i in range(len(gun_file)):
        gun_file[i].close()
        hit_file[i].close()

# read_folder = "sim_neuland_600MeV"
# save_folder = "600MeV"
read_folder = "sim_neuland_550-650MeV_100000_per_n"
save_folder = "1_neutrons_550_to_650_MeV_100k_events_per_neutron"
#save_folder = "1_neutron_550_to_650_MeV"

# Comment out files if less neutrons are wanted
gun_files = [#open('/net/home/marpol/land02/scripts/ggland/scripts/markus/' + read_folder + '/n0_on_neuland_gun.txt', 'r'),
             open('/net/home/marpol/land02/scripts/ggland/scripts/markus/' + read_folder + '/n1_on_neuland_gun_QGSP_BERT.txt', 'r')]
             #open('/net/home/marpol/land02/scripts/ggland/scripts/markus/' + read_folder + '/n2_on_neuland_gun_QGSP_BERT.txt', 'r'),
             #open('/net/home/marpol/land02/scripts/ggland/scripts/markus/' + read_folder + '/n3_on_neuland_gun_QGSP_BERT.txt', 'r'),
             #open('/net/home/marpol/land02/scripts/ggland/scripts/markus/' + read_folder + '/n4_on_neuland_gun_QGSP_BERT.txt', 'r'),
             #open('/net/home/marpol/land02/scripts/ggland/scripts/markus/' + read_folder + '/n5_on_neuland_gun_QGSP_BERT.txt', 'r'),
             #open('/net/home/marpol/land02/scripts/ggland/scripts/markus/' + read_folder + '/n6_on_neuland_gun_QGSP_BERT.txt', 'r'),
             #open('/net/home/marpol/land02/scripts/ggland/scripts/markus/' + read_folder + '/n7_on_neuland_gun_QGSP_BERT.txt', 'r')]

hit_files = [#open('/net/home/marpol/land02/scripts/ggland/scripts/markus/' + read_folder + '/n0_on_neuland_digi.txt', 'r'),
             open('/net/home/marpol/land02/scripts/ggland/scripts/markus/' + read_folder + '/n1_on_neuland_QGSP_BERT_digi.txt', 'r')]
             #open('/net/home/marpol/land02/scripts/ggland/scripts/markus/' + read_folder + '/n2_on_neuland_QGSP_BERT_digi.txt', 'r'),
             #open('/net/home/marpol/land02/scripts/ggland/scripts/markus/' + read_folder + '/n3_on_neuland_QGSP_BERT_digi.txt', 'r'),
             #open('/net/home/marpol/land02/scripts/ggland/scripts/markus/' + read_folder + '/n4_on_neuland_QGSP_BERT_digi.txt', 'r'),
             #open('/net/home/marpol/land02/scripts/ggland/scripts/markus/' + read_folder + '/n5_on_neuland_QGSP_BERT_digi.txt', 'r'),
             #open('/net/home/marpol/land02/scripts/ggland/scripts/markus/' + read_folder + '/n6_on_neuland_QGSP_BERT_digi.txt', 'r'),
             #open('/net/home/marpol/land02/scripts/ggland/scripts/markus/' + read_folder + '/n7_on_neuland_QGSP_BERT_digi.txt', 'r')]


filepath = '/net/home/marpol/python_code/cnnTest/sim_neuland_tfrecords/' + save_folder + '/n_on_neuland_'

ttv_distribution = [0, 90000, 95000, 100000]
# ttv_distribution = [0, 100000, 100000, 100000]
write_train_test_validation_files(False, gun_files, hit_files, filepath, ttv_distribution)


