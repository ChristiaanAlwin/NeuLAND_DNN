import neuland_readfile_dataset as nds
import numpy as np


# TF_RECORD_FOLDER = '5_neutrons_600MeV'
# TF_RECORD_FOLDER = "5_neutrons_550_to_650_MeV"
TF_RECORD_FOLDER = "7_neutrons_550_to_650_MeV_100k_events_per_neutron"
# TF_RECORD_FOLDER = "7_neutrons_550_to_650_MeV_100k_events_per_neutron_reduced_high_n_events"
# TF_RECORD_FOLDER = "7_neutrons_550_to_650_MeV_100k_events_per_neutron_minedep"
# TF_RECORD_FOLDER = "7_neutrons_550_to_650_MeV_100k_events_per_neutron_minedep_not_reduced"
# TF_RECORD_FOLDER = "7_neutrons_550_to_650_MeV_100k_events_per_neutron_minedep_birk_not_reduced"
# TF_RECORD_FOLDER = "7_neutrons_550_to_650_MeV_100k_events_per_neutron_5n_validation(for_minedep_trained)"
# TF_RECORD_FOLDER = "only_1_neutron_550_to_650_MeV"
# TF_RECORD_FOLDER = "1_neutrons_550_to_650_MeV_100k_events_per_neutron"

TRAIN_FILE = 'sim_neuland_tfrecords/' + TF_RECORD_FOLDER + '/n_on_neuland_train.tfrecords'
TEST_FILE = 'sim_neuland_tfrecords/' + TF_RECORD_FOLDER + '/n_on_neuland_test.tfrecords'
VALIDATION_FILE = 'sim_neuland_tfrecords/' + TF_RECORD_FOLDER + '/n_on_neuland_validation.tfrecords'


class Dataset(object):

    def __init__(self, file_path, flatten=True, start_stop_records=None, normalize=False, normalize_e=None, normalize_h=None, image_norm_t=None, image_norm_e=None):
        self._guns, self._hits, self._gun_event, self._hit_event, self._gun_one_hot, self._total_e = nds.get_all_records(file_path, start_stop_records=start_stop_records)

        self._total_hits = self.events_total_hits(self._hit_event)
        self._normalize_e = normalize_e
        self._normalize_h = normalize_h
        self._image_norm_t = image_norm_t
        self._image_norm_e = image_norm_e

        if normalize:
            # Set value interval to [0, 1]
            # Per image normalizing
            self.normalize_image("std")
            # Scale entire set equally
            # self._image_norm_t, self._image_norm_e = self.scale_entire_image_set(image_norm_t, image_norm_e)
            self._total_e, self._normalize_e = self.normalize(self._total_e, self._normalize_e)
            self._total_hits, self._normalize_h = self.normalize(self._total_hits, self._normalize_h)

        self._epochs_completed = 0
        self._index_in_epoch = 0
        self._num_examples = np.size(self._guns)
        self._flatten = flatten

    @property
    def normalize_e(self):
        return self._normalize_e

    @property
    def normalize_h(self):
        return self._normalize_h

    @property
    def image_norm_t(self):
        return self._image_norm_t

    @property
    def image_norm_e(self):
        return self._image_norm_e

    # If hit_list is false a full [50*50*60*2] image is returned
    # if hit_list is true a list of the hits is returned
    def hit_event(self, subset=None, hit_list=False):
        if subset is None:
            subset = [0, self._num_examples]
        if hit_list:
            return self._hit_event[subset[0]:subset[1]]

        return self.events_to_image(self._hit_event[subset[0]:subset[1]], self._flatten)

    def gun_one_hot(self, subset=None):
        if subset is None:
            subset = [0, self._num_examples]
        return self._gun_one_hot[subset[0]:subset[1]]

    def gun_event(self, subset=None):
        if subset is None:
            subset = [0, self._num_examples]
        return self._gun_event[subset[0]:subset[1]]

    def num_examples(self):
        return self._num_examples

    def epochs_completed(self):
        return self._epochs_completed

    def total_e(self, subset=None):
        if subset is None:
            subset = [0, self._num_examples]
        return self._total_e[subset[0]:subset[1]]

    def total_hits(self, subset=None):
        if subset is None:
            subset = [0, self._num_examples]
        return self._total_hits[subset[0]:subset[1]]

    def total_e_and_hits(self, subset=None):
        if subset is None:
            subset = [0, self._num_examples]
        return np.vstack((self._total_e[subset[0]:subset[1]], self._total_hits[subset[0]:subset[1]])).T

    def guns(self, subset=None):
        if subset is None:
            subset = [0, self._num_examples]
        return self._guns[subset[0]:subset[1]]

    def normalize(self, v, normalize_val):
        if normalize_val is None:
            largest = np.amax(v)
            if largest == 0:
                return v, largest
            return v / largest, largest

        if normalize_val == 0:
            return v, normalize_val

        return v / normalize_val, normalize_val

    def scale_entire_image_set(self, norm_val_t, norm_val_e):
        max_t, max_e = self.largest_t_and_e(self._hit_event)
        if norm_val_t is None or norm_val_e is None:
            for i in range(np.size(self._hit_event)):
                # scale total set
                self._hit_event[i] /= [1, 1, 1, max_t, max_e]
                return max_t, max_e
        else:
            for i in range(np.size(self._hit_event)):
                # scale total set
                self._hit_event[i] /= [1, 1, 1, norm_val_t, norm_val_e]
                return norm_val_t, norm_val_e

    # normalize each image
    def normalize_image(self, norm_type="std"):
        for i in range(np.size(self._hit_event)):
            # Normalize total set
            # self._hit_event[i] /= [1, 1, 1, max_t, max_e]

            # Normalize each image, divide by max
            if norm_type == "max":
                max_t, max_e = self.largest_t_and_e_event(self._hit_event[i])
                self._hit_event[i] /= [1, 1, 1, max_t, max_e]

            # Normalize each image, divide by std and 0 mean shift
            if norm_type == "std":
                if np.size(self._hit_event[i]) > 0:
                    # Compute mean if more than 1 value otherwise zero shift
                    mean_t = np.mean(self._hit_event[i][:, 3])
                    mean_e = np.mean(self._hit_event[i][:, 4])
                    std_t = np.std(self._hit_event[i][:, 3])
                    std_e = np.std(self._hit_event[i][:, 4])

                    # if standard deviation is zero, set to mean and set mean to zero
                    if std_t == 0:
                        std_t = mean_t
                        mean_t = 0
                    if std_e == 0:
                        std_e = mean_e
                        mean_e = 0

                    if std_t == 0 or std_e == 0:
                        print('step: %d, %s, %s' % (i, std_t, std_e))
                        print(self._hit_event[i])
                    self._hit_event[i] -= [0, 0, 0, mean_t, mean_e]
                    self._hit_event[i] /= [1.0, 1.0, 1.0, std_t, std_e]
                    if std_t == 0 or std_e == 0:
                        print(self._hit_event[i])

    # gets largest time and energy of all hit events
    def largest_t_and_e(self, hit_events):
        e = 0
        t = 0
        for event in hit_events:
            t_tmp, e_tmp = self.largest_t_and_e_event(event)
            if t_tmp > t:
                t = t_tmp
            if e_tmp > e:
                e = e_tmp

        return t, e

    # gets largest time and energy of hit event
    def largest_t_and_e_event(self, hit_event):
        e = 0
        t = 0
        for hit in hit_event:
            if np.size(hit) > 0:  # can be zero and must have special treatment...
                if hit[3] > t:
                    t = hit[3]
                if hit[4] > e:
                    e = hit[4]

        return t, e

    # returns a new batch of total energy, total hits, event image, one-hot gun vector and gun_event vector
    # shuffles the training data in each epoch
    def next_batch(self, batch_size, shuffle=True, total_e_only=False):
        """Return the next `batch_size` examples from this data set."""
        start = self._index_in_epoch

        # Shuffle for the first epoch
        if self._epochs_completed == 0 and start == 0 and shuffle:
            perm0 = np.arange(self._num_examples)
            np.random.shuffle(perm0)
            self._hit_event = self._hit_event[perm0]
            self._gun_one_hot = self._gun_one_hot[perm0]
            self._total_e = self._total_e[perm0]
            self._total_hits = self._total_hits[perm0]
            self._gun_event = self._gun_event[perm0]

        # Go to the next epoch
        if start + batch_size > self._num_examples:
            # Finished epoch
            self._epochs_completed += 1
            # Get the rest examples in this epoch
            rest_num_examples = self._num_examples - start
            hit_event_rest_part = self._hit_event[start:self._num_examples]
            gun_one_hot_rest_part = self._gun_one_hot[start:self._num_examples]
            total_e_rest_part = self._total_e[start:self._num_examples]
            total_hits_rest_part = self._total_hits[start:self._num_examples]
            gun_event_rest_part = self._gun_event[start:self._num_examples]

            # Shuffle the data
            if shuffle:
                perm = np.arange(self._num_examples)
                np.random.shuffle(perm)
                self._hit_event = self._hit_event[perm]
                self._gun_one_hot = self._gun_one_hot[perm]
                self._total_e = self._total_e[perm]
                self._total_hits = self._total_hits[perm]
                self._gun_event = self._gun_event[perm]

            # Start next epoch
            start = 0
            self._index_in_epoch = batch_size - rest_num_examples
            end = self._index_in_epoch
            hit_event_new_part = self._hit_event[start:end]
            gun_one_hot_new_part = self._gun_one_hot[start:end]
            total_e_new_part = self._total_e[start:end]
            total_hits_new_part = self._total_hits[start:end]
            gun_event_new_part = self._gun_event[start:end]

            ret_1 = np.concatenate((total_e_rest_part, total_e_new_part), axis=0)
            ret_2 = np.concatenate((gun_one_hot_rest_part, gun_one_hot_new_part), axis=0)
            ret_3 = np.concatenate((total_hits_rest_part, total_hits_new_part), axis=0)
            ret_5 = np.concatenate((gun_event_rest_part, gun_event_new_part), axis=0)

            # return only total e and total hits if true
            if total_e_only:
                return np.vstack((ret_1, ret_3)).T, ret_2

            #  Convert hit events into 4d image
            tmp_1 = self.events_to_image(hit_event_rest_part, self._flatten)
            tmp_2 = self.events_to_image(hit_event_new_part, self._flatten)
            ret_4 = np.concatenate((tmp_1, tmp_2), axis=0)

            return ret_1, ret_3, ret_4, ret_2, ret_5

        else:
            self._index_in_epoch += batch_size
            end = self._index_in_epoch

            ret_1 = self._total_e[start:end]
            ret_2 = self._gun_one_hot[start:end]
            ret_3 = self._total_hits[start:end]
            ret_5 = self._gun_event[start:end]

            # return only total e and total hits if true
            if total_e_only:
                return np.vstack((ret_1, ret_3)).T, ret_2

            #  Convert hit events into 4d image
            ret_4 = self.events_to_image(self._hit_event[start:end], self._flatten)

            return ret_1, ret_3, ret_4, ret_2, ret_5

    # Converts each event into hit image
    def event_hits_to_image(self, hits, flatten):
        #hits = np.reshape(hits, (-1, 5))
        # create 4d image [x:(0-49), y:(0-49), z:(0-59), t|e]
        event_image = np.zeros((50, 50, 60, 2))

        for hit in hits:
            # skip if some number is not a number, ehh...
            if np.isnan(hit[0]) or np.isnan(hit[1]) or np.isnan(hit[2]) or np.isnan(hit[3]) or np.isnan(hit[4]):
                continue

            event_image[int(hit[0])][int(hit[1])][int(hit[2])][0] = hit[3]
            event_image[int(hit[0])][int(hit[1])][int(hit[2])][1] = hit[4]
        if flatten:
            event_image = event_image.flatten()

        return event_image

    #  Converts hits into 4d images. If flatten is true, the 4d image is flattened to 1d
    def events_to_image(self, events, flatten=True):
        if flatten:
            event_list = np.zeros((np.size(events), 300000))
        else:
            event_list = np.zeros((np.size(events), 50, 50, 60, 2))

        for i in range(np.size(events)):
            event_list[i] = self.event_hits_to_image(events[i], flatten)

        return event_list

    # Gets total hits of all events
    def events_total_hits(self, events):
        event_list = np.zeros(np.size(events))
        for i in range(np.size(events)):
            event_list[i] = self.get_total_hits(events[i])

        return event_list

    # Counts total hits of single event
    def get_total_hits(self, hits):
        #hits = np.reshape(hits, (-1, 5))
        total_hits = 0
        for hit in hits:
            # skip if some number is not a number, ehh...
            if np.isnan(hit[0]) or np.isnan(hit[1]) or np.isnan(hit[2]) or np.isnan(hit[3]) or np.isnan(hit[4]):
                continue

            total_hits += 1

        return total_hits


class NeulandDataset(object):

    def __init__(self, flatten=True, start_stop_records=None, normalize=False):
        print('Loading from file: train set')
        self._train = Dataset(TRAIN_FILE, flatten, start_stop_records=start_stop_records, normalize=normalize)
        print('Loading from file: validation set')
        self._validation = Dataset(VALIDATION_FILE, flatten, start_stop_records=None, normalize=normalize, normalize_e=self.train.normalize_e, normalize_h=self.train.normalize_h, image_norm_t=self.train.image_norm_t, image_norm_e=self.train.image_norm_e)
        print('Loading from file: test set')
        self._test = Dataset(TEST_FILE, flatten, start_stop_records=None, normalize=normalize, normalize_e=self.train.normalize_e, normalize_h=self.train.normalize_h, image_norm_t=self.train.image_norm_t, image_norm_e=self.train.image_norm_e)

    @property
    def train(self):
        return self._train

    @property
    def validation(self):
        return self._validation

    @property
    def test(self):
        return self._test
