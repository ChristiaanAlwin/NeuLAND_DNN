
import tensorflow as tf
import models_2 as mds
import numpy as np


class ModelHandler(object):

    def __init__(self, global_params):

        self._global_step = 0
        self._global_params = global_params
        self._training = True

        # Input placeholders
        with tf.name_scope('input'):
            self._hit_image = tf.placeholder(tf.float32, [None, 50, 50, 60, 2], name='hit_image-input')
            self._tot_e_and_h = tf.placeholder(tf.float32, [None, 2], name='tot_e_and_h-input')
            self._neutron_number_one_hot = tf.placeholder(tf.float32, [None, self._global_params['neutron_number_possibilities']], name='neutron_number-input')
            # self._neutron_momenta = tf.placeholder(tf.float32, [None, 3], name='neutron_momenta-input')

        self._global_step_op = tf.contrib.framework.get_or_create_global_step()

        # Exponential decay
        self._learning_rate = tf.train.exponential_decay(global_params['learning_rate_start'], self._global_step_op, decay_steps=global_params['decay_steps'], decay_rate=global_params['decay_rate'])

        # Set the network architecture here. Prediction of network.
        # self._neutron_number_prediction_one_hot = mds.simple_multilayer_cnn_2_neutron_multiplicity(self._global_params, hit_image=self._hit_image, tot_e_and_h=self._tot_e_and_h)
        self._neutron_number_prediction_one_hot = mds.simple_multilayer_cnn_3_tot_e_and_h_neutron_multiplicity(self._global_params, hit_image=self._hit_image, tot_e_and_h=self._tot_e_and_h)
        # self._neutron_number_prediction_one_hot = mds.total_e_and_h_only(self._global_params, tot_e_and_h=self._tot_e_and_h)

        # for momentum prediciton
        # self._neutron_momenta_prediction = mds.simple_multilayer_cnn_2_neutron_momentum(self._global_params, self._hit_image)

        # Loss function
        self._loss_function = softmax_cross_entropy_neutron_number(self._neutron_number_one_hot, self._neutron_number_prediction_one_hot)

        # for momentum prediciton
        # self._loss_function = mse_neutron_momentum(self._neutron_momenta, self._neutron_momenta_prediction)

        # Optimizer for minimizing the loss function
        self._train_opt = tf.train.AdamOptimizer(self._learning_rate)

        self._train_step = self._train_opt.minimize(self._loss_function, global_step=self._global_step_op)

        # Vector with predicted neutron number for each event
        self._neutron_number_prediction = tf.argmax(self._neutron_number_prediction_one_hot, 1)
        # Vector with correct neutron number for each event
        self._neutron_number = tf.argmax(self._neutron_number_one_hot, 1)

        # A vector with ones for the events with correct neutron number prediction and zeros for the failed events.
        self._correct_prediction_neutron_number = tf.equal(self._neutron_number_prediction, self._neutron_number)

        # Total percentage of predicting the correct number of neutrons.
        self._accuracy = tf.reduce_mean(tf.cast(self._correct_prediction_neutron_number, tf.float32))
        tf.summary.scalar('accuracy', self._accuracy)

        hooks = tf.train.StopAtStepHook(last_step=self._global_params['max_train_steps'])

        self._sess = tf.train.MonitoredTrainingSession(checkpoint_dir=self._global_params['model_save_path'],
                                                       save_summaries_steps=self._global_params['save_steps'],
                                                       config=tf.ConfigProto(allow_soft_placement=True),
                                                       hooks=[hooks])

    @property
    def training(self):
        return self._training

    @property
    def global_step(self):
        return self._global_step

    def train_batch(self, data_set, batch_size):
        batch_tot_e, batch_tot_h, batch_hit_image, batch_neutron_number_one_hot, batch_neutron_momenta = data_set.next_batch(batch_size)
        batch_e_and_h = np.vstack((batch_tot_e, batch_tot_h)).T
        feed_dict = {self._tot_e_and_h: batch_e_and_h,
                     self._hit_image: batch_hit_image,
                     self._neutron_number_one_hot: batch_neutron_number_one_hot,
                     # self._neutron_momenta: batch_neutron_momenta[:, 1:4]
                     }
        self._global_step, _ = self._sess.run([self._global_step_op, self._train_step], feed_dict=feed_dict)

        if self._sess.should_stop():
            # end training
            self._training = False

    # Split testing into smaller batches to avoid large memory usage
    # returns accuracy and separation matrix over whole test_set
    def test(self, data_set, test_set_size=None, batch_size=100):
        if test_set_size == 0:
            number_of_batches = int(data_set.num_examples() / batch_size)
        else:
            number_of_batches = int(test_set_size / batch_size)
            if number_of_batches > data_set.num_examples() / batch_size:
                number_of_batches = int(data_set.num_examples() / batch_size)

        beg = 0
        end = batch_size
        acc_tot = 0

        neutron_sep_mat = np.zeros((self._global_params['neutron_number_possibilities'], self._global_params['neutron_number_possibilities']))

        # # for momentum prediction
        # neutron_sep_mat = "None"

        norm_tensor = np.zeros(self._global_params['neutron_number_possibilities'])

        for test_batch in range(int(number_of_batches)):
            print("Testing batch %s of %s" % (test_batch + 1, number_of_batches))
            feed_dict = {self._tot_e_and_h: data_set.total_e_and_hits([beg, end]),
                         self._hit_image: data_set.hit_event([beg, end]),
                         self._neutron_number_one_hot: data_set.gun_one_hot([beg, end]),
                         # self._neutron_momenta: data_set.gun_event([beg, end])[:, 1:4]
                         }
            acc_tot_tmp, neutron_number_prediction, neutron_number = \
                self._sess.run([self._accuracy, self._neutron_number_prediction, self._neutron_number], feed_dict=feed_dict)

            # for momentum prediction
            # acc_tot_tmp = self._sess.run(self._loss_function, feed_dict=feed_dict)

            acc_tot += acc_tot_tmp
            neutron_sep_mat, norm_tensor = neutron_separation_matrix_append(neutron_sep_mat, norm_tensor, neutron_number_prediction, neutron_number)

            print('Current average accuracy: %s' % float(acc_tot/(test_batch + 1)))
            beg = end
            end += batch_size

        # normalize
        neutron_sep_mat /= norm_tensor[None, :]
        acc_tot /= float(number_of_batches)

        return acc_tot, neutron_sep_mat


def neutron_separation_matrix_append(neutron_sep_mat, norm_tensor, neutron_number_prediction, neutron_number):
    for i in range(len(neutron_number_prediction)):
        norm_tensor[neutron_number[i]] += 1
        neutron_sep_mat[neutron_number_prediction[i], neutron_number[i]] += 1

    return neutron_sep_mat, norm_tensor


def softmax_cross_entropy_neutron_number(neutron_number_one_hot, neutron_number_prediction_one_hot):

    # Softmax cross entropy
    diff = tf.nn.softmax_cross_entropy_with_logits(labels=neutron_number_one_hot, logits=neutron_number_prediction_one_hot)
    cross_entropy = tf.reduce_mean(diff)
    tf.summary.scalar('cross_entropy', cross_entropy)

    return cross_entropy


# Gives the mean squared error of the momentum
def mse_neutron_momentum(neutron_momentum, neutron_momentum_prediction):
    # loss function
    diff_x = tf.squared_difference(neutron_momentum_prediction[0], neutron_momentum[0])
    diff_y = tf.squared_difference(neutron_momentum_prediction[1], neutron_momentum[1])
    diff_z = tf.squared_difference(neutron_momentum_prediction[2], neutron_momentum[2])

    loss_mse_x = tf.reduce_mean(diff_x)
    loss_mse_y = tf.reduce_mean(diff_y)
    loss_mse_z = tf.reduce_mean(diff_z)

    tf.summary.scalar('loss_mse_x', loss_mse_x)
    tf.summary.scalar('loss_mse_y', loss_mse_y)
    tf.summary.scalar('loss_mse_z', loss_mse_z)

    loss_mse = tf.reduce_mean(diff_x + diff_y + diff_z)
    tf.summary.scalar('loss_mse', loss_mse)

    return loss_mse

