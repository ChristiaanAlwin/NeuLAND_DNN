import numpy as np
import tensorflow as tf

'''
TRAIN_FILE = '/net/home/marpol/land02/scripts/ggland/scripts/markus/sim_neuland_tfrecords/n_on_neuland_train.tfrecords'
TEST_FILE = '/net/home/marpol/land02/scripts/ggland/scripts/markus/sim_neuland_tfrecords/n_on_neuland_test.tfrecords'
VALIDATION_FILE = '/net/home/marpol/land02/scripts/ggland/scripts/markus/sim_neuland_tfrecords/n_on_neuland_validation.tfrecords'


def get_batch(data_set, batch_size):
    if data_set == 'train':
        return input_pipeline([TRAIN_FILE], batch_size)
    elif data_set == 'test':
        return input_pipeline([TEST_FILE], batch_size)
    elif data_set == 'validation':
        return input_pipeline([VALIDATION_FILE], batch_size)


def input_pipeline(filenames, batch_size, num_epochs=None):
    filename_queue = tf.train.string_input_producer(filenames, num_epochs=num_epochs, shuffle=True)
    gun_n, hit_n, guns, hits, gun_one_hot, total_e = read_and_decode(filename_queue)

    # min_after_dequeue defines how big a buffer we will randomly sample
    #   from -- bigger means better shuffling but slower start up and more
    #   memory used.
    # capacity must be larger than min_after_dequeue and the amount larger
    #   determines the maximum we will prefetch.  Recommendation:
    #   min_after_dequeue + (num_threads + a small safety margin) * batch_size
    min_after_dequeue = 100
    capacity = min_after_dequeue + 3 * batch_size
    gun_n_batch, hit_n_batch, guns_batch, hits_batch, gun_one_hot_batch, total_e_batch = tf.train.shuffle_batch(
        [gun_n, hit_n, guns, hits, gun_one_hot, total_e], batch_size=batch_size,
        capacity=capacity, min_after_dequeue=min_after_dequeue)

    return gun_n_batch, hit_n_batch, guns_batch, hits_batch, gun_one_hot_batch, total_e_batch
'''


def read_and_decode(filename_queue):
    reader = tf.TFRecordReader()
    _, serialized_example = reader.read(filename_queue)

    features = tf.parse_single_example(
        serialized_example,
        features={'guns_raw': tf.FixedLenFeature([], tf.string),
                  'hits_raw': tf.FixedLenFeature([], tf.string),
                  'gun_one_hot': tf.FixedLenFeature([], tf.string),
                  'guns': tf.FixedLenFeature([], tf.int64),
                  'hits': tf.FixedLenFeature([], tf.int64),
                  'total_e': tf.FixedLenFeature([], tf.float32)}
    )

    guns_raw = tf.decode_raw(features['guns_raw'], tf.float64)
    hits_raw = tf.decode_raw(features['hits_raw'], tf.float64)
    gun_one_hot = tf.decode_raw(features['gun_one_hot'], tf.float64)
    guns = features['guns']
    hits = features['hits']
    total_e = features['total_e']

    # guns_raw = tf.reshape(guns_raw, [6, 4])
    # hits_raw = tf.reshape(hits_raw, [50, 50, 60, 2])
    # hits_raw = tf.reshape(hits_raw, [300000])
    # gun_one_hot = tf.reshape(gun_one_hot, [6])
    # guns = tf.reshape(guns, [1])
    # hits = tf.reshape(hits, [1])

    return guns, hits, guns_raw, hits_raw, gun_one_hot, total_e


def get_all_records(file, start_stop_records=None):
    with tf.Session(config=tf.ConfigProto(allow_soft_placement=True)) as sess:
        filename_queue = tf.train.string_input_producer([file], num_epochs=1)
        guns, hits, gun_event, hit_event, gun_one_hot, total_e = read_and_decode(filename_queue)

        guns_all, hits_all, gun_event_all, hit_event_all, gun_one_hot_all, total_e_all = [], [], [], [], [], []

        # The op for initializing the variables.
        init_op = tf.group(tf.global_variables_initializer(),
                           tf.local_variables_initializer())

        sess.run(init_op)
        coord = tf.train.Coordinator()
        threads = tf.train.start_queue_runners(coord=coord)

        record_num = -1
        try:
            while not coord.should_stop():
                if start_stop_records is not None:
                    record_num += 1
                    if record_num < start_stop_records[0]:
                        # Skip event in file
                        sess.run([guns, hits, gun_event, hit_event, gun_one_hot, total_e])
                        continue
                    elif record_num >= start_stop_records[1]:
                        break

                guns_val, hits_val, gun_event_val, hit_event_val, gun_one_hot_val, total_e_val = sess.run([guns, hits,
                                                                                                           gun_event,
                                                                                                           hit_event,
                                                                                                           gun_one_hot,
                                                                                                           total_e])

                guns_all.append(guns_val)
                hits_all.append(hits_val)
                gun_event_all.append(gun_event_val)
                hit_event_all.append(np.reshape(hit_event_val, (-1, 5)))
                gun_one_hot_all.append(gun_one_hot_val)
                total_e_all.append(total_e_val)

        except tf.errors.OutOfRangeError:
            print('Loaded file')
        finally:
            # When done, ask the threads to stop.
            coord.request_stop()

        # Wait for threads to finish.
        coord.join(threads)
        sess.close()

        #  return guns_all, hits_all, gun_event_all, hit_event_all, gun_one_hot_all
        return (np.array(guns_all), np.array(hits_all), np.array(gun_event_all),
                np.array(hit_event_all), np.array(gun_one_hot_all), np.array(total_e_all))


# get_all_records(TRAIN_FILE)
