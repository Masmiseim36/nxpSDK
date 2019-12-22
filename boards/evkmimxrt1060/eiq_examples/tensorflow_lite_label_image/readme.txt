Overview
========
TensorFlow Lite model based implementation of object detector based on
TensorFlow Lite example [2] adjusted to run on MCUs.

A 3-channel color image is set as an input to a quantized Mobilenet
convolutional neural network model [1] that classifies the input image into
one of 1000 output classes.

Firstly a static stopwatch image is set as input regardless camera is connected or not.
Secondly runtime image processing from camera in the case camera and LCD
is connected. Camera data are displayed on LCD.

HOW TO USE THE APPLICATION:
To classify an image, place an image in front of camera so that it fits in the
white rectangle in the middle of the LCD.
Note semihosting implementation causes slower or discontinuous video experience. 
Select UART in 'Project Options' for using external debug console 
via UART (Virtual COM port).

[1] https://www.tensorflow.org/lite/models
[2] https://github.com/tensorflow/tensorflow/tree/r1.14/tensorflow/lite/examples/label_image

Files:
  label_image.cpp - example source code
  bitmap_helpers.cpp - BMP image format decoding functions
  bitmap_helpers.h - BMP image format decoding function prototypes
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
  get_top_n.h - get top result prototypes
  get_top_n_impl - get top result source code
  timer.c - timer function source code
  timer.h - timer function prototypes
  image.c - image processing function source code
  image.h - image processing function prototypes


Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- Keil MDK  5.29
- GCC ARM Embedded  8.3.1
- MCUXpresso  11.1.0

Hardware requirements
=====================
- No camera input
    - Mini/micro USB cable
    - EVK-MIMXRT1060 board
    - Personal computer

- Camera input
    - Mini/micro USB cable
    - EVK-MIMXRT1060 board
    - Personal computer
    - Camera MT9M114
    - Liquid crystal display RK043FN02H-CT

Board settings
==============
- No camera input
    - No special settings are required

- Camera input
    - Move jumper J1 to position 1-2
    - Connect external 5V power supply to J2 connector

Prepare the demo
1. Connect a USB cable between the host PC and the OpenSDA USB port on the target board. 
2. Connect a camera to J35 connector. (Skip this step in the case offline version is used.)
3. Connect a LCD display to A1-A40 and B1-B6. (Skip this step in the case offline version is used.)
4. Open a serial terminal with the following settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
5. Download the program to the target board.

Prepare the Demo
================

Running the demo
================
The log below shows the output of the demo in the terminal window (compiled with ARM GCC):

Label image example using a TensorFlow Lite model.
Detection threshold: (25%)

Static data processing:
----------------------------------------
     Inference time: 301 ms
     Detected: stopwatch (90%)
----------------------------------------

Camera data processing:
----------------------------------------
     Inference time: 328 ms
     Detected: dishrag (55%)
----------------------------------------

----------------------------------------
     Inference time: 328 ms
     Detected: screw (26%)
----------------------------------------

----------------------------------------
     Inference time: 335 ms
     Detected: tree frog (91%)
----------------------------------------

----------------------------------------
     Inference time: 331 ms
     Detected: flamingo (54%)
----------------------------------------

----------------------------------------
     Inference time: 327 ms
     Detected: tiger (41%)
----------------------------------------

----------------------------------------
     Inference time: 328 ms
     Detected: snail (27%)
----------------------------------------
Customization options
=====================

