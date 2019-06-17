Overview
========
Demonstrates a convolutional neural network (CNN) example with the use of
convolution, ReLU activation, pooling and fully-connected functions.
The CNN model in the example was trained using the scripts available at [1]
with the CifarNet model. The configuration of the model was modified to match
the neural network structure in the CMSIS-NN CIFAR-10 example.
The example source code is a modified version of the Label Image
example from the TensorFlow Lite examples [2], adjusted to run on MCUs.
The neural network consists of 3 convolution layers interspersed by
ReLU activation and max pooling layers, followed by a fully-connected layer
at the end. The input to the network is a 32x32 pixel color image, which is 
classified into one of the 10 output classes. The model size is 91 KB. 

[1] https://github.com/tensorflow/models/tree/master/research/slim
[2] https://github.com/tensorflow/tensorflow/tree/r1.11/tensorflow/contrib/lite/examples/label_image

Files:
  cifar10.cpp - example source code
  cifar10_model.tflite - pre-trained TensorFlow Lite model
  cifar10_model.h - model data converted to a C language array
    from the cifar10_model.tflite file using the xxd tool (distributed
    with the Vim editor at www.vim.org)
  ship.bmp - shrinked picture of the object to recognize
    (source: https://en.wikipedia.org/wiki/File:Christian_Radich_aft_foto_Ulrich_Grun.jpg)
  ship_image.h - picture from ship.bmp converted into a C language array
    of RGB values using Python with the OpenCV and Numpy packages:
    import cv2
    import numpy as np
    img = cv2.imread('ship.bmp')
    img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    with open('ship_image.h', 'w') as fout:
      print('const char ship_image[] = {', file=fout)
      img.tofile(fout, ', ', '0x%2x')
      print('};\n', file=fout)


Toolchain supported
===================
- IAR embedded Workbench  8.32.3
- Keil MDK  5.27
- MCUXpresso  11.0.0
- GCC ARM Embedded  8.2.1

Hardware requirements
=====================
- Mini/micro USB cable
- EVKB-IMXRT1050 or EVK-MIMXRT1060 board
- Personal computer

Board settings
==============
No special settings are required.

Prepare the demo
1. Connect a USB cable between the host PC and the OpenSDA USB port on the target board. 
2. Open a serial terminal with the following settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.

Prepare the Demo
================

Running the demo
================
The log below shows the output of the demo in the terminal window (compiled with ARM GCC):

CIFAR-10 object recognition example using a TensorFlow Lite model
Elapsed time: 55 ms
Detected: ship (100% confidence)

Customization options
=====================

