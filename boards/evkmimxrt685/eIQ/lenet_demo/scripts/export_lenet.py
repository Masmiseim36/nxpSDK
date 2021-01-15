#
# Copyright 2019-2020 NXP
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import argparse
import os
import urllib

import numpy as np
import tensorflow as tf
from tensorflow.contrib.layers import flatten
from tensorflow.examples.tutorials.mnist import input_data

"""
Parse arguments
"""

parser = argparse.ArgumentParser()
parser.add_argument('--batch_size', default=128, type=int, help='batch size')
parser.add_argument('--epochs', default=50, type=int, help='number of epochs')
parser.add_argument('--train_steps', default=100, type=int, help='number of training steps')
parser.add_argument('--learning_rate', default=0.001, type=float, help='number of training steps')
parser.add_argument('--workdir', default=os.path.dirname(os.path.abspath(__file__)), type=str, help='working directory')
parser.add_argument('--dummy', default=False, type=bool, help='Use dummy zeros as weights instead of the real ones')


def main(argv):
    args = parser.parse_args(argv[1:])

    """
    Download MNIST data if needed 
    """

    DATA_DIR = os.path.join(args.workdir, 'data')
    if not os.path.exists(DATA_DIR):
        os.mkdir(DATA_DIR)

        MNIST_URL = 'http://yann.lecun.com/exdb/mnist/'
        MNIST_ARCHIVES = [
            'train-images-idx3-ubyte.gz',
            'train-labels-idx1-ubyte.gz',
            't10k-images-idx3-ubyte.gz',
            't10k-labels-idx1-ubyte.gz'
        ]

        # Download archives
        for mnist_archive in MNIST_ARCHIVES:
            filepath, _ = urllib.request.urlretrieve(MNIST_URL + mnist_archive, os.path.join(DATA_DIR, mnist_archive))
            print('Downloaded: %s' % mnist_archive)

    """
    Import MNIST
    """

    mnist = input_data.read_data_sets("./data/", reshape=False)

    def _general_dataset(X, y, is_training=False):
        dataset = tf.data.Dataset.from_tensor_slices((X, y))
        if is_training:
            dataset = dataset.shuffle(buffer_size=128)
        dataset = dataset.batch(args.batch_size)
        if is_training:
            dataset = dataset.repeat()
        iterator = dataset.make_initializable_iterator()
        next_element = iterator.get_next()
        return iterator, next_element

    """
        MODEL
    """

    def init_layer(W_shape, b_shape, name):
        W = tf.truncated_normal(shape=W_shape, mean=0, stddev=0.1)
        W = tf.Variable(W, name=name + '_W')

        b = tf.zeros(b_shape)
        b = tf.Variable(b, name=name + '_b')

        return W, b

    x = tf.placeholder(tf.float32, (None, 28, 28, 1))
    y = tf.placeholder(tf.int32, None)
    one_hot_y = tf.one_hot(y, 10)

    # 28x28x1 -> 24x24x20.
    conv1_W, conv1_b = init_layer(W_shape=(5, 5, 1, 20), b_shape=20, name='conv1')
    conv1 = tf.nn.conv2d(x, conv1_W, strides=[1, 1, 1, 1], padding='VALID') + conv1_b
    conv1 = tf.nn.relu(conv1)
    # 24x24x20 -> 12x12x20.
    conv1 = tf.nn.max_pool(conv1, ksize=[1, 2, 2, 1], strides=[1, 2, 2, 1], padding='VALID')

    # 12x12x20 -> 8x8x50.
    conv2_W, conv2_b = init_layer(W_shape=(5, 5, 20, 50), b_shape=50, name='conv2')
    conv2 = tf.nn.conv2d(conv1, conv2_W, strides=[1, 1, 1, 1], padding='VALID') + conv2_b
    conv2 = tf.nn.relu(conv2)
    # 8x8x50 -> 4x4x50
    conv2 = tf.nn.max_pool(conv2, ksize=[1, 2, 2, 1], strides=[1, 2, 2, 1], padding='VALID')
    # 4x4x50 -> 800
    fc0 = flatten(conv2)

    # 800 -> 500
    fc1_W, fc1_b = init_layer(W_shape=(800, 500), b_shape=500, name='fc1')
    fc1 = tf.matmul(fc0, fc1_W) + fc1_b
    fc1 = tf.nn.relu(fc1)

    # 500 -> 10
    fc2_W, fc2_b = init_layer(W_shape=(500, 10), b_shape=10, name='fc2')
    fc2 = tf.matmul(fc1, fc2_W) + fc2_b

    logits = fc2

    """
        TRAIN AND EVALUATE
    """

    MODEL_DIR = os.path.join(args.workdir, 'model')

    cross_entropy = tf.nn.softmax_cross_entropy_with_logits(logits=logits, labels=one_hot_y)
    loss_operation = tf.reduce_mean(cross_entropy)
    optimizer = tf.train.AdamOptimizer(learning_rate=args.learning_rate)
    training_operation = optimizer.minimize(loss_operation)

    correct_prediction = tf.equal(tf.argmax(logits, 1), tf.argmax(one_hot_y, 1))
    accuracy_operation = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))
    saver = tf.train.Saver()

    def evaluate(X_data, y_data):
        num_examples = len(X_data)
        total_accuracy = 0
        iterator, next_element = _general_dataset(X_data, y_data, is_training=False)

        sess = tf.get_default_session()
        sess.run(iterator.initializer)
        while True:
            try:
                batch_x, batch_y = sess.run(next_element)
                accuracy = sess.run(accuracy_operation, feed_dict={x: batch_x, y: batch_y})
                total_accuracy += (accuracy * len(batch_x))
            except tf.errors.OutOfRangeError:
                break

        return total_accuracy / num_examples

    if os.path.exists(MODEL_DIR):
        print("Model already trained")
    else:
        os.makedirs(MODEL_DIR)

        with tf.Session() as sess:
            iterator, next_element = _general_dataset(mnist.train.images, mnist.train.labels, is_training=True)

            sess.run(tf.global_variables_initializer())
            sess.run(iterator.initializer)

            print("Training...")
            for epoch in range(args.epochs):
                for iter in range(args.train_steps):
                    batch_x, batch_y = sess.run(next_element)
                    _, loss = sess.run([training_operation, loss_operation], feed_dict={x: batch_x, y: batch_y})

                accuracy = evaluate(mnist.validation.images, mnist.validation.labels)
                print("EPOCH %2d: accuracy = %f, loss= %f" % (epoch + 1, accuracy, loss))

            saver.save(sess, os.path.join(MODEL_DIR, 'lenet'))
            print("Finished training")

    with tf.Session() as sess:
        saver.restore(sess, tf.train.latest_checkpoint(MODEL_DIR))
        test_accuracy = evaluate(mnist.test.images, mnist.test.labels)
        print("Test Accuracy = %f" % test_accuracy)

    """
        EXPORT WEIGHTS AND BIASES
    """

    WEIGHTS_DIR = os.path.join(args.workdir, os.path.join('test_bin', 'weights'))
    if not os.path.exists(WEIGHTS_DIR):
        os.makedirs(WEIGHTS_DIR)

    def create_Cadence_kernel4conv(kernel):
        input_channels = kernel.shape[2]
        input_channels_pad = (input_channels + 2 - 1) & ~(2 - 1)
        kernel_pad = np.pad(kernel, ((0, 0), (0, 0), (0, input_channels_pad - input_channels), (0, 0)), 'constant',
                            constant_values=(0, 0))
        kernel_pad = kernel_pad.transpose((3, 0, 1, 2))
        return kernel_pad

    def create_Cadence_kernel4fc(kernel):
        input_channels = kernel.shape[0]
        input_channels_pad = (input_channels + 2 - 1) & ~(2 - 1)
        kernel_pad = np.pad(kernel, ((0, input_channels_pad - input_channels), (0, 0)), 'constant',
                            constant_values=(0, 0))
        kernel_pad = kernel_pad.transpose((1, 0))
        return kernel_pad

    with tf.Session() as sess:
        saver.restore(sess, tf.train.latest_checkpoint(MODEL_DIR))

        all_vars = [conv1_W, conv1_b, conv2_W, conv2_b, fc1_W, fc1_b, fc2_W, fc2_b]
        all_vars = {var.name.split(':')[0]: np.asarray(var.eval(session=sess)) for var in all_vars}

        all_vars['conv1_W'] = create_Cadence_kernel4conv(all_vars['conv1_W'])
        all_vars['conv2_W'] = create_Cadence_kernel4conv(all_vars['conv2_W'])
        all_vars['fc1_W'] = create_Cadence_kernel4fc(all_vars['fc1_W'])
        all_vars['fc2_W'] = create_Cadence_kernel4fc(all_vars['fc2_W'])

        for layer_name, arr in all_vars.items():
            with open(os.path.join(WEIGHTS_DIR, layer_name + '.bin'), "wb") as f:
                arr.tofile(f)

    """
        EXPORT INPUTS AND OUTPUTS
    """

    IO_DIR = os.path.join(args.workdir, os.path.join('test_bin', 'io'))
    if not os.path.exists(IO_DIR):
        os.makedirs(IO_DIR)

    with open(os.path.join(IO_DIR, "input1.bin"), "wb") as f:
        mnist.test.images[:1].tofile(f)

    with open(os.path.join(IO_DIR, "input100.bin"), "wb") as f:
        mnist.test.images[:100].tofile(f)

    with open(os.path.join(IO_DIR, "output.bin"), "wb") as f:
        mnist.test.labels[:100].tofile(f)

    """
        EXPORT CODE
    """

    for folder in ['weights', 'io']:
        path_in = os.path.join(args.workdir, os.path.join('test_bin', folder))
        path_out = os.path.join(args.workdir, 'test_h')

        if not os.path.exists(path_out):
            os.makedirs(path_out)

        files = [file for file in os.listdir(path_in) if file.endswith(".bin")]
        for file in files:
            variable = os.path.splitext(os.path.basename(file))[0]
            with open(os.path.join(path_in, file), "rb") as f:
                content = f.read()
                content = [hex(byte) for byte in content]

            with open(os.path.join(path_out, variable + ".h"), "wt") as h:
                h.write("#ifndef __%s_H__\n" % variable.upper())
                h.write("#define __%s_H__\n" % variable.upper())
                h.write("\n")
                h.write("__attribute__((aligned(8))) const unsigned char %s[%d] = {" % (variable, len(content)))
                if args.dummy:
                    h.write("0")
                else:
                    h.write(", ".join(content))
                h.write("};\n")
                h.write("\n")
                h.write("#endif //__%s_H__\n" % variable.upper())


if __name__ == '__main__':
    tf.logging.set_verbosity(tf.logging.INFO)
    tf.app.run(main)
