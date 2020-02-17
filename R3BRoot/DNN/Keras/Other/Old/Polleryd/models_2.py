import tensorflow as tf
import placeholder_functions as pf
import datetime


C_VAC = 299792458.0


def write_architecture_summary(summary_file, architecture_params):
    summary = '\n\nStart run: ' + datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S") + '\n'

    for k, v in sorted(architecture_params.items()):
        summary += k + ': ' + str(v) + '\n'

    f = open(summary_file, 'a+')
    f.write(summary)
    f.close()


# Required params:
# global_params = {'neutron_number_possibilities': neutron_number_possibilities,
#                  'save_directory': save_directory}
def simple_multilayer_cnn_2_neutron_multiplicity(global_params, hit_image, tot_e_and_h):

    architecture_params = {'W_conv1_1': [3, 3, 3, 2, 16], 'W_conv1_2': [3, 3, 3, 16, 16], 'pool_1': 'avg',
                           'W_conv2_1': [3, 3, 3, 16, 32], 'W_conv2_2': [3, 3, 3, 32, 32], 'pool_2': 'avg',
                           'W_conv3_1': [3, 3, 3, 32, 64], 'W_conv3_2': [3, 3, 3, 64, 64], 'pool_3': 'avg',
                           'fc_1_neurons': 12544, 'fc_2_neurons': 12544}

    write_architecture_summary(global_params['save_directory'] + 'architecture_params', architecture_params)

    # Create a multilayer model.

    # Pattern recognition network
    # Convolution layer 1
    # conv 1
    W_conv1_1 = pf.weight_variable(architecture_params['W_conv1_1'])
    b_conv1_1 = pf.bias_variable([architecture_params['W_conv1_1'][4]])
    h_conv1_1 = pf.tf.nn.relu(pf.conv3d(hit_image, W_conv1_1) + b_conv1_1)
    # conv 2
    W_conv1_2 = pf.weight_variable(architecture_params['W_conv1_2'])
    b_conv1_2 = pf.bias_variable([architecture_params['W_conv1_2'][4]])
    h_conv1_2 = pf.tf.nn.relu(pf.conv3d(h_conv1_1, W_conv1_2) + b_conv1_2)
    h_pool1 = pf.pool_2x2x2(h_conv1_2, architecture_params['pool_1'])

    # Convolution layer 2
    # conv 1
    W_conv2_1 = pf.weight_variable(architecture_params['W_conv2_1'])
    b_conv2_1 = pf.bias_variable([architecture_params['W_conv2_1'][4]])
    h_conv2_1 = pf.tf.nn.relu(pf.conv3d(h_pool1, W_conv2_1) + b_conv2_1)
    # conv 2
    W_conv2_2 = pf.weight_variable(architecture_params['W_conv2_2'])
    b_conv2_2 = pf.bias_variable([architecture_params['W_conv2_2'][4]])
    h_conv2_2 = pf.tf.nn.relu(pf.conv3d(h_conv2_1, W_conv2_2) + b_conv2_2)
    h_pool2 = pf.pool_2x2x2(h_conv2_2, architecture_params['pool_2'])

    # Convolution layer 3
    # conv 1
    W_conv3_1 = pf.weight_variable(architecture_params['W_conv3_1'])
    b_conv3_1 = pf.bias_variable([architecture_params['W_conv3_1'][4]])
    h_conv3_1 = pf.tf.nn.relu(pf.conv3d(h_pool2, W_conv3_1) + b_conv3_1)
    # conv2
    W_conv3_2 = pf.weight_variable(architecture_params['W_conv3_2'])
    b_conv3_2 = pf.bias_variable([architecture_params['W_conv3_2'][4]])
    h_conv3_2 = pf.tf.nn.relu(pf.conv3d(h_conv3_1, W_conv3_2) + b_conv3_2)
    h_pool3 = pf.pool_2x2x2(h_conv3_2, architecture_params['pool_3'])

    # Fully-connected layer 1
    size = 7 * 7 * 8 * architecture_params['W_conv3_2'][4]

    W_fc1 = pf.weight_variable([size, architecture_params['fc_1_neurons']])
    b_fc1 = pf.bias_variable([architecture_params['fc_1_neurons']])
    h_pool3_flat = tf.reshape(h_pool3, [-1, size])
    h_fc1 = tf.nn.relu(tf.matmul(h_pool3_flat, W_fc1) + b_fc1)

    # Fully-connected layer 2
    W_fc2 = pf.weight_variable([architecture_params['fc_1_neurons'], architecture_params['fc_2_neurons']])
    b_fc2 = pf.bias_variable([architecture_params['fc_2_neurons']])
    h_fc2 = tf.nn.relu(tf.matmul(h_fc1, W_fc2) + b_fc2)

    # Readout layer
    W_fc3 = pf.weight_variable([architecture_params['fc_2_neurons'], global_params['neutron_number_possibilities']])
    b_fc3 = pf.bias_variable([global_params['neutron_number_possibilities']])

    # The networks approximation of a one-hot vector representing the number of neutrons
    neutron_number_prediction_one_hot = tf.matmul(h_fc2, W_fc3) + b_fc3

    return neutron_number_prediction_one_hot


# Required params:
# global_params = {'neutron_number_possibilities': neutron_number_possibilities,
#                  'save_directory': save_directory}
def simple_multilayer_cnn_2_neutron_momentum(global_params, hit_image):

    architecture_params = {'W_conv1_1': [3, 3, 3, 2, 16], 'W_conv1_2': [3, 3, 3, 16, 16], 'pool_1': 'avg',
                           'W_conv2_1': [3, 3, 3, 16, 32], 'W_conv2_2': [3, 3, 3, 32, 32], 'pool_2': 'avg',
                           'W_conv3_1': [3, 3, 3, 32, 64], 'W_conv3_2': [3, 3, 3, 64, 64], 'pool_3': 'avg',
                           'fc_1_neurons': 12544, 'fc_2_neurons': 12544}

    write_architecture_summary(global_params['save_directory'] + 'architecture_params', architecture_params)

    # Create a multilayer model.

    # Pattern recognition network
    # Convolution layer 1
    # conv 1
    W_conv1_1 = pf.weight_variable(architecture_params['W_conv1_1'])
    b_conv1_1 = pf.bias_variable([architecture_params['W_conv1_1'][4]])
    h_conv1_1 = pf.tf.nn.relu(pf.conv3d(hit_image, W_conv1_1) + b_conv1_1)
    # conv 2
    W_conv1_2 = pf.weight_variable(architecture_params['W_conv1_2'])
    b_conv1_2 = pf.bias_variable([architecture_params['W_conv1_2'][4]])
    h_conv1_2 = pf.tf.nn.relu(pf.conv3d(h_conv1_1, W_conv1_2) + b_conv1_2)
    h_pool1 = pf.pool_2x2x2(h_conv1_2, architecture_params['pool_1'])

    # Convolution layer 2
    # conv 1
    W_conv2_1 = pf.weight_variable(architecture_params['W_conv2_1'])
    b_conv2_1 = pf.bias_variable([architecture_params['W_conv2_1'][4]])
    h_conv2_1 = pf.tf.nn.relu(pf.conv3d(h_pool1, W_conv2_1) + b_conv2_1)
    # conv 2
    W_conv2_2 = pf.weight_variable(architecture_params['W_conv2_2'])
    b_conv2_2 = pf.bias_variable([architecture_params['W_conv2_2'][4]])
    h_conv2_2 = pf.tf.nn.relu(pf.conv3d(h_conv2_1, W_conv2_2) + b_conv2_2)
    h_pool2 = pf.pool_2x2x2(h_conv2_2, architecture_params['pool_2'])

    # Convolution layer 3
    # conv 1
    W_conv3_1 = pf.weight_variable(architecture_params['W_conv3_1'])
    b_conv3_1 = pf.bias_variable([architecture_params['W_conv3_1'][4]])
    h_conv3_1 = pf.tf.nn.relu(pf.conv3d(h_pool2, W_conv3_1) + b_conv3_1)
    # conv2
    W_conv3_2 = pf.weight_variable(architecture_params['W_conv3_2'])
    b_conv3_2 = pf.bias_variable([architecture_params['W_conv3_2'][4]])
    h_conv3_2 = pf.tf.nn.relu(pf.conv3d(h_conv3_1, W_conv3_2) + b_conv3_2)
    h_pool3 = pf.pool_2x2x2(h_conv3_2, architecture_params['pool_3'])

    # Fully-connected layer 1
    size = 7 * 7 * 8 * architecture_params['W_conv3_2'][4]

    W_fc1 = pf.weight_variable([size, architecture_params['fc_1_neurons']])
    b_fc1 = pf.bias_variable([architecture_params['fc_1_neurons']])
    h_pool3_flat = tf.reshape(h_pool3, [-1, size])
    h_fc1 = tf.nn.relu(tf.matmul(h_pool3_flat, W_fc1) + b_fc1)

    # Fully-connected layer 2
    W_fc2 = pf.weight_variable([architecture_params['fc_1_neurons'], architecture_params['fc_2_neurons']])
    b_fc2 = pf.bias_variable([architecture_params['fc_2_neurons']])
    h_fc2 = tf.nn.relu(tf.matmul(h_fc1, W_fc2) + b_fc2)

    # Readout layer
    W_fc3 = pf.weight_variable([architecture_params['fc_2_neurons'], 3])
    b_fc3 = pf.bias_variable([3])

    neutron_momentum_prediction = tf.matmul(h_fc2, W_fc3) + b_fc3

    return neutron_momentum_prediction


# Required params:
# global_params = {'neutron_number_possibilities': neutron_number_possibilities,
#                  'save_directory': save_directory}
def simple_multilayer_cnn_2_neutron_momentum_from_position(global_params, hit_image):

    architecture_params = {'W_conv1_1': [3, 3, 3, 2, 16], 'W_conv1_2': [3, 3, 3, 16, 16], 'pool_1': 'avg',
                           'W_conv2_1': [3, 3, 3, 16, 32], 'W_conv2_2': [3, 3, 3, 32, 32], 'pool_2': 'avg',
                           'W_conv3_1': [3, 3, 3, 32, 64], 'W_conv3_2': [3, 3, 3, 64, 64], 'pool_3': 'avg',
                           'fc_1_neurons': 12544, 'fc_2_neurons': 12544}

    write_architecture_summary(global_params['save_directory'] + 'architecture_params', architecture_params)

    # Create a multilayer model.

    # Pattern recognition network
    # Convolution layer 1
    # conv 1
    W_conv1_1 = pf.weight_variable(architecture_params['W_conv1_1'])
    b_conv1_1 = pf.bias_variable([architecture_params['W_conv1_1'][4]])
    h_conv1_1 = pf.tf.nn.relu(pf.conv3d(hit_image, W_conv1_1) + b_conv1_1)
    # conv 2
    W_conv1_2 = pf.weight_variable(architecture_params['W_conv1_2'])
    b_conv1_2 = pf.bias_variable([architecture_params['W_conv1_2'][4]])
    h_conv1_2 = pf.tf.nn.relu(pf.conv3d(h_conv1_1, W_conv1_2) + b_conv1_2)
    h_pool1 = pf.pool_2x2x2(h_conv1_2, architecture_params['pool_1'])

    # Convolution layer 2
    # conv 1
    W_conv2_1 = pf.weight_variable(architecture_params['W_conv2_1'])
    b_conv2_1 = pf.bias_variable([architecture_params['W_conv2_1'][4]])
    h_conv2_1 = pf.tf.nn.relu(pf.conv3d(h_pool1, W_conv2_1) + b_conv2_1)
    # conv 2
    W_conv2_2 = pf.weight_variable(architecture_params['W_conv2_2'])
    b_conv2_2 = pf.bias_variable([architecture_params['W_conv2_2'][4]])
    h_conv2_2 = pf.tf.nn.relu(pf.conv3d(h_conv2_1, W_conv2_2) + b_conv2_2)
    h_pool2 = pf.pool_2x2x2(h_conv2_2, architecture_params['pool_2'])

    # Convolution layer 3
    # conv 1
    W_conv3_1 = pf.weight_variable(architecture_params['W_conv3_1'])
    b_conv3_1 = pf.bias_variable([architecture_params['W_conv3_1'][4]])
    h_conv3_1 = pf.tf.nn.relu(pf.conv3d(h_pool2, W_conv3_1) + b_conv3_1)
    # conv2
    W_conv3_2 = pf.weight_variable(architecture_params['W_conv3_2'])
    b_conv3_2 = pf.bias_variable([architecture_params['W_conv3_2'][4]])
    h_conv3_2 = pf.tf.nn.relu(pf.conv3d(h_conv3_1, W_conv3_2) + b_conv3_2)
    h_pool3 = pf.pool_2x2x2(h_conv3_2, architecture_params['pool_3'])

    # Fully-connected layer 1
    size = 7 * 7 * 8 * architecture_params['W_conv3_2'][4]

    W_fc1 = pf.weight_variable([size, architecture_params['fc_1_neurons']])
    b_fc1 = pf.bias_variable([architecture_params['fc_1_neurons']])
    h_pool3_flat = tf.reshape(h_pool3, [-1, size])
    h_fc1 = tf.nn.relu(tf.matmul(h_pool3_flat, W_fc1) + b_fc1)

    # Fully-connected layer 2
    W_fc2 = pf.weight_variable([architecture_params['fc_1_neurons'], architecture_params['fc_2_neurons']])
    b_fc2 = pf.bias_variable([architecture_params['fc_2_neurons']])
    h_fc2 = tf.nn.relu(tf.matmul(h_fc1, W_fc2) + b_fc2)

    # Readout layer
    W_fc3 = pf.weight_variable([architecture_params['fc_2_neurons'], 4])
    b_fc3 = pf.bias_variable([4])

    neutron_position_prediction = tf.matmul(h_fc2, W_fc3) + b_fc3


    # Calculate momentum
    v_x = neutron_position_prediction[:, 0] / neutron_position_prediction[:, 3]
    v_y = neutron_position_prediction[:, 1] / neutron_position_prediction[:, 3]
    v_z = neutron_position_prediction[:, 2] / neutron_position_prediction[:, 3]

    k = 1.0 / C_VAC / tf.sqrt(1.0 - (v_x * v_x + v_y * v_y + v_z * v_z) / C_VAC)
    print(k.get_shape())

    p_x = k * v_x
    p_y = k * v_y
    p_z = k * v_z

    return tf.stack([p_x, p_y, p_z], axis=1)


# Required params:
# global_params = {'neutron_number_possibilities': neutron_number_possibilities,
#                  'save_directory': save_directory}
def simple_multilayer_cnn_2_tot_e_and_h_neutron_multiplicity(global_params, hit_image, tot_e_and_h):

    architecture_params = {'W_conv1_1': [3, 3, 3, 2, 16], 'W_conv1_2': [3, 3, 3, 16, 16], 'pool_1': 'avg',
                           'W_conv2_1': [3, 3, 3, 16, 32], 'W_conv2_2': [3, 3, 3, 32, 32], 'pool_2': 'avg',
                           'W_conv3_1': [3, 3, 3, 32, 64], 'W_conv3_2': [3, 3, 3, 64, 64], 'pool_3': 'avg',
                           'fc_1_neurons': 12544, 'fc_2_neurons': 12544}  # 12544, 8000, 4096

    write_architecture_summary(global_params['save_directory'] + 'architecture_params', architecture_params)

    # Create a multilayer model.

    # Pattern recognition network
    # Convolution layer 1
    # conv 1
    W_conv1_1 = pf.weight_variable(architecture_params['W_conv1_1'])
    b_conv1_1 = pf.bias_variable([architecture_params['W_conv1_1'][4]])
    h_conv1_1 = pf.tf.nn.relu(pf.conv3d(hit_image, W_conv1_1) + b_conv1_1)  # ReLU unit
    # h_conv1_1 = pf.tf.nn.sigmoid(pf.conv3d(hit_image, W_conv1_1) + b_conv1_1)  # Sigmoid unit
    # conv 2
    W_conv1_2 = pf.weight_variable(architecture_params['W_conv1_2'])
    b_conv1_2 = pf.bias_variable([architecture_params['W_conv1_2'][4]])
    h_conv1_2 = pf.tf.nn.relu(pf.conv3d(h_conv1_1, W_conv1_2) + b_conv1_2)  # ReLU unit
    # h_conv1_2 = pf.tf.nn.sigmoid(pf.conv3d(h_conv1_1, W_conv1_2) + b_conv1_2)  # Sigmoid unit
    h_pool1 = pf.pool_2x2x2(h_conv1_2, architecture_params['pool_1'])

    # Convolution layer 2
    # conv 1
    W_conv2_1 = pf.weight_variable(architecture_params['W_conv2_1'])
    b_conv2_1 = pf.bias_variable([architecture_params['W_conv2_1'][4]])
    h_conv2_1 = pf.tf.nn.relu(pf.conv3d(h_pool1, W_conv2_1) + b_conv2_1)  # ReLU unit
    # h_conv2_1 = pf.tf.nn.sigmoid(pf.conv3d(h_pool1, W_conv2_1) + b_conv2_1)  # Sigmoid unit
    # conv 2
    W_conv2_2 = pf.weight_variable(architecture_params['W_conv2_2'])
    b_conv2_2 = pf.bias_variable([architecture_params['W_conv2_2'][4]])
    h_conv2_2 = pf.tf.nn.relu(pf.conv3d(h_conv2_1, W_conv2_2) + b_conv2_2)  # ReLU unit
    # h_conv2_2 = pf.tf.nn.sigmoid(pf.conv3d(h_conv2_1, W_conv2_2) + b_conv2_2)  # Sigmoid unit
    h_pool2 = pf.pool_2x2x2(h_conv2_2, architecture_params['pool_2'])

    # Convolution layer 3
    # conv 1
    W_conv3_1 = pf.weight_variable(architecture_params['W_conv3_1'])
    b_conv3_1 = pf.bias_variable([architecture_params['W_conv3_1'][4]])
    h_conv3_1 = pf.tf.nn.relu(pf.conv3d(h_pool2, W_conv3_1) + b_conv3_1)  # ReLU unit
    # h_conv3_1 = pf.tf.nn.sigmoid(pf.conv3d(h_pool2, W_conv3_1) + b_conv3_1)  # Sigmoid unit
    # conv2
    W_conv3_2 = pf.weight_variable(architecture_params['W_conv3_2'])
    b_conv3_2 = pf.bias_variable([architecture_params['W_conv3_2'][4]])
    h_conv3_2 = pf.tf.nn.relu(pf.conv3d(h_conv3_1, W_conv3_2) + b_conv3_2)  # ReLU unit
    # h_conv3_2 = pf.tf.nn.sigmoid(pf.conv3d(h_conv3_1, W_conv3_2) + b_conv3_2)  # Sigmoid unit
    h_pool3 = pf.pool_2x2x2(h_conv3_2, architecture_params['pool_3'])

    # Fully-connected layer 1
    size = 7 * 7 * 8 * architecture_params['W_conv3_2'][4]

    W_fc1 = pf.weight_variable([size + 2, architecture_params['fc_1_neurons']])
    b_fc1 = pf.bias_variable([architecture_params['fc_1_neurons']])
    h_pool3_flat = tf.reshape(h_pool3, [-1, size])

    # merge flattened image with tot_e_and_h
    h_pool3_flat_merged = tf.concat([h_pool3_flat, tot_e_and_h], 1)
    h_fc1 = tf.nn.relu(tf.matmul(h_pool3_flat_merged, W_fc1) + b_fc1)  # ReLU unit
    # h_fc1 = tf.nn.sigmoid(tf.matmul(h_pool3_flat_merged, W_fc1) + b_fc1)  # Sigmoid unit

    # Fully-connected layer 2
    W_fc2 = pf.weight_variable([architecture_params['fc_1_neurons'], architecture_params['fc_2_neurons']])
    b_fc2 = pf.bias_variable([architecture_params['fc_2_neurons']])
    h_fc2 = tf.nn.relu(tf.matmul(h_fc1, W_fc2) + b_fc2)  # ReLU unit
    # h_fc2 = tf.nn.sigmoid(tf.matmul(h_fc1, W_fc2) + b_fc2)  # Sigmoid unit

    # Readout layer
    W_fc3 = pf.weight_variable([architecture_params['fc_2_neurons'], global_params['neutron_number_possibilities']])
    b_fc3 = pf.bias_variable([global_params['neutron_number_possibilities']])

    # The networks approximation of a one-hot vector representing the number of neutrons
    neutron_number_prediction_one_hot = tf.matmul(h_fc2, W_fc3) + b_fc3

    return neutron_number_prediction_one_hot


# Required params:
# global_params = {'neutron_number_possibilities': neutron_number_possibilities,
#                  'save_directory': save_directory}
def simple_multilayer_cnn_3_tot_e_and_h_neutron_multiplicity(global_params, hit_image, tot_e_and_h):

    architecture_params = {'W_conv1_1': [3, 3, 3, 2, 16], 'W_conv1_2': [3, 3, 3, 16, 16], 'pool_1': 'avg',
                           'W_conv2_1': [3, 3, 3, 16, 32], 'W_conv2_2': [3, 3, 3, 32, 32], 'pool_2': 'avg',
                           'W_conv3_1': [3, 3, 3, 32, 64], 'W_conv3_2': [3, 3, 3, 64, 64], 'pool_3': 'avg',
                           'W_conv4_1': [3, 3, 3, 64, 128], 'W_conv4_2': [3, 3, 3, 128, 128], 'pool_4': 'avg',
                           'W_conv5_1': [3, 3, 3, 128, 256], 'W_conv5_2': [3, 3, 3, 256, 256], 'pool_5': 'avg',
                           'fc_1_neurons': 2048, 'fc_2_neurons': 2048, 'fc_3_neurons': 2048}

    write_architecture_summary(global_params['save_directory'] + 'architecture_params', architecture_params)

    # Create a multilayer model.

    # Pattern recognition network
    # Convolution layer 1
    # conv 1
    W_conv1_1 = pf.weight_variable(architecture_params['W_conv1_1'])
    b_conv1_1 = pf.bias_variable([architecture_params['W_conv1_1'][4]])
    h_conv1_1 = pf.tf.nn.relu(pf.conv3d(hit_image, W_conv1_1) + b_conv1_1)  # ReLU unit
    # h_conv1_1 = pf.tf.nn.sigmoid(pf.conv3d(hit_image, W_conv1_1) + b_conv1_1)  # Sigmoid unit
    # conv 2
    W_conv1_2 = pf.weight_variable(architecture_params['W_conv1_2'])
    b_conv1_2 = pf.bias_variable([architecture_params['W_conv1_2'][4]])
    h_conv1_2 = pf.tf.nn.relu(pf.conv3d(h_conv1_1, W_conv1_2) + b_conv1_2)  # ReLU unit
    # h_conv1_2 = pf.tf.nn.sigmoid(pf.conv3d(h_conv1_1, W_conv1_2) + b_conv1_2)  # Sigmoid unit
    h_pool1 = pf.pool_2x2x2(h_conv1_2, architecture_params['pool_1'])

    # Convolution layer 2
    # conv 1
    W_conv2_1 = pf.weight_variable(architecture_params['W_conv2_1'])
    b_conv2_1 = pf.bias_variable([architecture_params['W_conv2_1'][4]])
    h_conv2_1 = pf.tf.nn.relu(pf.conv3d(h_pool1, W_conv2_1) + b_conv2_1)  # ReLU unit
    # h_conv2_1 = pf.tf.nn.sigmoid(pf.conv3d(h_pool1, W_conv2_1) + b_conv2_1)  # Sigmoid unit
    # conv 2
    W_conv2_2 = pf.weight_variable(architecture_params['W_conv2_2'])
    b_conv2_2 = pf.bias_variable([architecture_params['W_conv2_2'][4]])
    h_conv2_2 = pf.tf.nn.relu(pf.conv3d(h_conv2_1, W_conv2_2) + b_conv2_2)  # ReLU unit
    # h_conv2_2 = pf.tf.nn.sigmoid(pf.conv3d(h_conv2_1, W_conv2_2) + b_conv2_2)  # Sigmoid unit
    h_pool2 = pf.pool_2x2x2(h_conv2_2, architecture_params['pool_2'])

    # Convolution layer 3
    # conv 1
    W_conv3_1 = pf.weight_variable(architecture_params['W_conv3_1'])
    b_conv3_1 = pf.bias_variable([architecture_params['W_conv3_1'][4]])
    h_conv3_1 = pf.tf.nn.relu(pf.conv3d(h_pool2, W_conv3_1) + b_conv3_1)  # ReLU unit
    # h_conv3_1 = pf.tf.nn.sigmoid(pf.conv3d(h_pool2, W_conv3_1) + b_conv3_1)  # Sigmoid unit
    # conv2
    W_conv3_2 = pf.weight_variable(architecture_params['W_conv3_2'])
    b_conv3_2 = pf.bias_variable([architecture_params['W_conv3_2'][4]])
    h_conv3_2 = pf.tf.nn.relu(pf.conv3d(h_conv3_1, W_conv3_2) + b_conv3_2)  # ReLU unit
    # h_conv3_2 = pf.tf.nn.sigmoid(pf.conv3d(h_conv3_1, W_conv3_2) + b_conv3_2)  # Sigmoid unit
    h_pool3 = pf.pool_2x2x2(h_conv3_2, architecture_params['pool_3'])

    # Convolution layer 4
    # conv 1
    W_conv4_1 = pf.weight_variable(architecture_params['W_conv4_1'])
    b_conv4_1 = pf.bias_variable([architecture_params['W_conv4_1'][4]])
    h_conv4_1 = pf.tf.nn.relu(pf.conv3d(h_pool3, W_conv4_1) + b_conv4_1)  # ReLU unit
    # h_conv4_1 = pf.tf.nn.sigmoid(pf.conv3d(h_pool3, W_conv4_1) + b_conv4_1)  # Sigmoid unit
    # conv2
    W_conv4_2 = pf.weight_variable(architecture_params['W_conv4_2'])
    b_conv4_2 = pf.bias_variable([architecture_params['W_conv4_2'][4]])
    h_conv4_2 = pf.tf.nn.relu(pf.conv3d(h_conv4_1, W_conv4_2) + b_conv4_2)  # ReLU unit
    # h_conv4_2 = pf.tf.nn.sigmoid(pf.conv3d(h_conv4_1, W_conv4_2) + b_conv4_2)  # Sigmoid unit
    h_pool4 = pf.pool_2x2x2(h_conv4_2, architecture_params['pool_4'])

    # Convolution layer 5
    # conv 1
    W_conv5_1 = pf.weight_variable(architecture_params['W_conv5_1'])
    b_conv5_1 = pf.bias_variable([architecture_params['W_conv5_1'][4]])
    h_conv5_1 = pf.tf.nn.relu(pf.conv3d(h_pool4, W_conv5_1) + b_conv5_1)  # ReLU unit
    # h_conv5_1 = pf.tf.nn.sigmoid(pf.conv3d(h_pool4, W_conv5_1) + b_conv5_1)  # Sigmoid unit
    # conv2
    W_conv5_2 = pf.weight_variable(architecture_params['W_conv5_2'])
    b_conv5_2 = pf.bias_variable([architecture_params['W_conv5_2'][4]])
    h_conv5_2 = pf.tf.nn.relu(pf.conv3d(h_conv5_1, W_conv5_2) + b_conv5_2)  # ReLU unit
    # h_conv5_2 = pf.tf.nn.sigmoid(pf.conv3d(h_conv5_1, W_conv5_2) + b_conv5_2)  # Sigmoid unit
    h_pool5 = pf.pool_2x2x2(h_conv5_2, architecture_params['pool_5'])

    # Fully-connected layer 1
    size = 2 * 2 * 2 * architecture_params['W_conv5_2'][4]

    W_fc1 = pf.weight_variable([size + 2, architecture_params['fc_1_neurons']])
    b_fc1 = pf.bias_variable([architecture_params['fc_1_neurons']])
    h_pool5_flat = tf.reshape(h_pool5, [-1, size])

    # merge flattened image with tot_e_and_h
    h_pool5_flat_merged = tf.concat([h_pool5_flat, tot_e_and_h], 1)
    h_fc1 = tf.nn.relu(tf.matmul(h_pool5_flat_merged, W_fc1) + b_fc1)  # ReLU unit
    # h_fc1 = tf.nn.sigmoid(tf.matmul(h_pool5_flat_merged, W_fc1) + b_fc1)  # Sigmoid unit

    # Fully-connected layer 2
    W_fc2 = pf.weight_variable([architecture_params['fc_1_neurons'], architecture_params['fc_2_neurons']])
    b_fc2 = pf.bias_variable([architecture_params['fc_2_neurons']])
    h_fc2 = tf.nn.relu(tf.matmul(h_fc1, W_fc2) + b_fc2)  # ReLU unit
    # h_fc2 = tf.nn.sigmoid(tf.matmul(h_fc1, W_fc2) + b_fc2)  # Sigmoid unit

    # Fully-connected layer 3
    W_fc3 = pf.weight_variable([architecture_params['fc_2_neurons'], architecture_params['fc_3_neurons']])
    b_fc3 = pf.bias_variable([architecture_params['fc_3_neurons']])
    h_fc3 = tf.nn.relu(tf.matmul(h_fc2, W_fc3) + b_fc3)  # ReLU unit
    # h_fc3 = tf.nn.sigmoid(tf.matmul(h_fc2, W_fc3) + b_fc3)  # Sigmoid unit

    # Readout layer
    W_fc4 = pf.weight_variable([architecture_params['fc_3_neurons'], global_params['neutron_number_possibilities']])
    b_fc4 = pf.bias_variable([global_params['neutron_number_possibilities']])

    # The networks approximation of a one-hot vector representing the number of neutrons
    neutron_number_prediction_one_hot = tf.matmul(h_fc3, W_fc4) + b_fc4

    return neutron_number_prediction_one_hot


# Required params:
# global_params = {'neutron_number_possibilities': neutron_number_possibilities,
#                  'save_directory': save_directory}
def total_e_and_h_only(global_params, tot_e_and_h):

    architecture_params = {'fc_1_neurons': 128, 'fc_2_neurons': 128}

    write_architecture_summary(global_params['save_directory'] + 'architecture_params', architecture_params)

    # Fully-connected layer 1
    size = 2
    with tf.name_scope('layer_fc1'):
        W_fc1 = pf.weight_variable([size, architecture_params['fc_1_neurons']])
        b_fc1 = pf.bias_variable([architecture_params['fc_1_neurons']])
        in_fc1 = tf.reshape(tot_e_and_h, [-1, size])
        h_fc1 = tf.nn.relu(tf.matmul(in_fc1, W_fc1) + b_fc1)

    # Fully-connected layer 2
    with tf.name_scope('layer_fc2'):
        W_fc2 = pf.weight_variable([architecture_params['fc_1_neurons'], architecture_params['fc_2_neurons']])
        b_fc2 = pf.bias_variable([architecture_params['fc_2_neurons']])
        h_fc2 = tf.nn.relu(tf.matmul(h_fc1, W_fc2) + b_fc2)

    # Fully-connected layer 3
    '''
    with tf.name_scope('layer_fc3'):
        W_fc3 = pf.weight_variable([neurons, neurons2])
        b_fc3 = pf.bias_variable([neurons2])
        h_fc3 = tf.nn.relu(tf.matmul(h_fc2, W_fc3) + b_fc3)
    '''

    # Readout layer
    with tf.name_scope('layer_readout'):
        W_fcRO = pf.weight_variable([architecture_params['fc_2_neurons'], global_params['neutron_number_possibilities']])
        b_fcRO = pf.bias_variable([global_params['neutron_number_possibilities']])

        neutron_number_prediction_one_hot = tf.matmul(h_fc2, W_fcRO) + b_fcRO

    return neutron_number_prediction_one_hot



