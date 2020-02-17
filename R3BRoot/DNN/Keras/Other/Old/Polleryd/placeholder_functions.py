import tensorflow as tf


# We can't initialize these variables to 0 - the network will get stuck.
def weight_variable(shape):
    """Create a weight variable with appropriate initialization."""
    initial = tf.truncated_normal(shape, stddev=0.1)
    return tf.Variable(initial)


def bias_variable(shape):
    """Create a bias variable with appropriate initialization."""
    initial = tf.constant(0.1, shape=shape)
    return tf.Variable(initial)


def conv3d(x, W):
    return tf.nn.conv3d(x, W, strides=[1, 1, 1, 1, 1], padding='SAME')


def conv2d(x, W):
    return tf.nn.conv2d(x, W, strides=[1, 1, 1, 1], padding='SAME')


def max_pool_2x2(x):
    return tf.nn.max_pool(x, ksize=[1, 2, 2, 1], strides=[1, 2, 2, 1], padding='SAME')


def pool_2x2x2(x, pool_type):
    if pool_type == 'avg':
        return avg_pool_2x2x2(x)
    elif pool_type == 'max':
        return max_pool_2x2x2(x)


def max_pool_2x2x2(x):
    return tf.nn.max_pool3d(x, ksize=[1, 2, 2, 2, 1], strides=[1, 2, 2, 2, 1], padding='SAME')


def max_pool_3x3x3(x):
    return tf.nn.max_pool3d(x, ksize=[1, 3, 3, 3, 1], strides=[1, 3, 3, 3, 1], padding='SAME')


def max_pool_5x5x5(x):
    return tf.nn.max_pool3d(x, ksize=[1, 5, 5, 5, 1], strides=[1, 5, 5, 5, 1], padding='SAME')


def avg_pool_2x2x2(x):
    return tf.nn.avg_pool3d(x, ksize=[1, 2, 2, 2, 1], strides=[1, 2, 2, 2, 1], padding='SAME')


def avg_pool_3x3x3(x):
    return tf.nn.avg_pool3d(x, ksize=[1, 3, 3, 3, 1], strides=[1, 3, 3, 3, 1], padding='SAME')


def avg_pool_5x5x5(x):
    return tf.nn.avg_pool3d(x, ksize=[1, 5, 5, 5, 1], strides=[1, 5, 5, 5, 1], padding='SAME')


# Average over all inputs to one single output value
def avg_pool_total(x):
    return tf.nn.avg_pool3d(x, ksize=[1, 50, 50, 60, 1], strides=[1, 50, 50, 60, 1], padding='SAME')


def variable_summaries(var):
    """Attach a lot of summaries to a Tensor (for TensorBoard visualization)."""
    with tf.name_scope('summaries'):
        mean = tf.reduce_mean(var)
        tf.summary.scalar('mean', mean)
    with tf.name_scope('stddev'):
        stddev = tf.sqrt(tf.reduce_mean(tf.square(var - mean)))
    tf.summary.scalar('stddev', stddev)
    tf.summary.scalar('max', tf.reduce_max(var))
    tf.summary.scalar('min', tf.reduce_min(var))
    tf.summary.histogram('histogram', var)
