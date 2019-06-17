Overview
========
The example shows how a pre-trained TensorFlow Lite model can be loaded and used
for recognizing objects in images. The pre-trained and quantized Mobilenet model
is obtained from the TensorFlow Lite models web page [1]. The model was trained
to recognize 1000 classes of objects from an input image. The example source
code is a modified version of the Label Image example from the TensorFlow Lite
examples [2], adjusted to run on MCUs. The input image is expected to be a
3-channel color image of any size - the example code resizes the input to match
the model input tensor.

[1] https://www.tensorflow.org/lite/models
[2] https://github.com/tensorflow/tensorflow/tree/r1.11/tensorflow/contrib/lite/examples/label_image

Files:
  label_image.cpp - example source code
  bitmap_helpers.cpp - BMP image format decoding functions
  labels.h - names of object classes
  mobilenet_v1_0.25_128_quant.tflite - pre-trained TensorFlow Lite model
    (source: http://download.tensorflow.org/models/mobilenet_v1_2018_08_02/mobilenet_v1_0.25_128.tgz)
  mobilenet_v1_0.25_128_quant_model.h - model data from the .tflite file
    converted into a C language array using the xxd tool (distributed
    with the Vim editor at www.vim.org)
  stopwatch.bmp - image file of the object to recognize
    (source: https://commons.wikimedia.org/wiki/File:Stopwatch2.jpg)
  stopwatch_image.h - image file converted into a C language array using
    the xxd tool (distributed with the Vim editor at www.vim.org)


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

Label image example using a TensorFlow Lite model
Average time: 144 ms
Detected:
  stopwatch (77% confidence)
  magnetic compass (18% confidence)
  dial telephone (1% confidence)
  binoculars (0% confidence)
  scale (0% confidence)

Customization options
=====================

