#
# Copyright 2019 NXP
# All rights reserved.
# 
# SPDX-License-Identifier: BSD-3-Clause
#

import os
import numpy as np
import argparse
from imageio import imread

IMAGE_MODE_OPTS = ['neg1to1', '0to1', '0to255', 'neg128to127']
IMAGE_LAYOUT_OPTS = ['NHWC', 'NCHW']
IMAGE_CHANNEL_ORDER_OPTS = ['RGB', 'BGR']


# Performs pre-processing of a given image and serialization to text in order to include in the code.
# It requires the following parameters:
# - image_path - full path to the image
# - output_path - full path to the output file
# - image_mode - image normalization mode: '0to1', 'neg1to1'
# - image_layout - 'NCHW' or 'NHWC'
# - image_channel_order - 'RGB' or 'BGR'
# - num_batch - the image will be repeated in order to obtain a batch (default is 1)
# Function returns serialized image size (bytes).
def process_image(image_path, output_path, image_mode, image_layout, image_channel_order, num_batch):

    # Validate parameters
    assert os.path.isfile(image_path), 'Image path invalid!'
    assert image_mode in IMAGE_MODE_OPTS, 'Image mode invalid1'
    assert image_layout in IMAGE_LAYOUT_OPTS, 'Image layout invalid!'
    assert image_channel_order in IMAGE_CHANNEL_ORDER_OPTS, 'Image channel invalid!'

    # Read image as float, channel order RGB and HWC format.
    # For gray scale image add channel dimension
    img_data = imread(image_path).astype(np.float32)
    if img_data.ndim == 2:
        img_data = np.reshape(img_data, (img_data.shape[0], img_data.shape[1], 1))

    # If image has ALPHA channel drop it
    if img_data.shape[2] == 4:
        img_data = img_data[:, :, (0, 1, 2)]

    # Convert from RGB to BGR
    if image_channel_order == 'BGR':
        if img_data.shape[2] == 3:
            img_data = img_data[:, :, (2, 1, 0)]

    # Convert image from HWC to CHW
    if image_layout == 'NCHW':
        img_data = np.transpose(img_data, (2, 0, 1))

    # Image normalization
    if image_mode == 'neg1to1':
        range_min = -1.0
        range_max = 1.0
    elif image_mode == '0to1':
        range_min = 0.0
        range_max = 1.0
    elif image_mode == '0to255':
        range_min = 0.0
        range_max = 255.0
    elif image_mode == 'neg128to127':
        range_min = -128.0
        range_max = 127.0
    else:
        assert False, 'Image mode invalid!'
    scale = (range_max - range_min) / 255.0
    bias = range_min
    img_data = img_data * scale + bias

    # Repeat image for batch processing (resulting tensor is NCHW or NHWC)
    img_data = np.reshape(img_data, (1, img_data.shape[0], img_data.shape[1], img_data.shape[2]))
    img_data = np.repeat(img_data, num_batch, axis=0)
    img_data = np.reshape(img_data, (num_batch, img_data.shape[1], img_data.shape[2], img_data.shape[3]))

    # Serialize image batch
    img_data_bytes = bytearray(img_data.tobytes(order='C'))
    image_bytes_per_line = 20
    with open(output_path, 'wt') as f:
        idx = 0
        for byte in img_data_bytes:
            f.write('0X%02X, ' % byte)
            if idx % image_bytes_per_line == (image_bytes_per_line - 1):
                f.write('\n')
            idx = idx + 1

    # Return serialized image size
    return len(img_data_bytes)


if __name__ == '__main__':
    parser = argparse.ArgumentParser()

    parser.add_argument('-image-path', action='store', dest='image_path', required=True, help='Input image path.')

    parser.add_argument('-output-path', action='store', dest='output_path', required=True, help='Output file path.')

    parser.add_argument('-image-mode', action='store', dest='image_mode', default='0to255',
                        choices=IMAGE_MODE_OPTS, help='Image normalization mode. Default is 0to255.')

    parser.add_argument('-image-layout', action='store', dest='image_layout', default='NHWC',
                        choices=IMAGE_LAYOUT_OPTS, help='Image output layout. Default is NHWC.')

    parser.add_argument('-image-channel-order', action='store', dest='image_channel_order', default='RGB',
                        choices=IMAGE_CHANNEL_ORDER_OPTS, help='Image channel order. Default is RGB.')

    parser.add_argument('-input-batch', action='store', dest='input_batch', type=int, default=1,
                        help='Input batch size. Image will be repeated to obtain a batch worth of data. Default is 1.')

    args = parser.parse_args()

    img_size = process_image(image_path=args.image_path,
                             output_path=args.output_path,
                             image_mode=args.image_mode,
                             image_layout=args.image_layout,
                             image_channel_order=args.image_channel_order,
                             num_batch=args.input_batch)

    print('Image processed to file \"%s\" ...' % args.output_path)
    print('Image size = %d (bytes)' % img_size)
