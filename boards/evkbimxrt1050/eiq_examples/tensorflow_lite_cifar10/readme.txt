Overview
========
Convolutional neural network (CNN) example with the use of
convolution, ReLU activation, pooling and fully-connected functions.

The CNN model in the example was trained using the scripts available at [1]
with the CifarNet model. 
The configuration of the model was modified to match the neural 
network structure in the CMSIS-NN CIFAR-10 example.
The example source code is a modified version of the Label Image
example from the TensorFlow Lite examples [2], adjusted to run on MCUs.
The neural network consists of 3 convolution layers interspersed by
ReLU activation and max pooling layers, followed by a fully-connected layer
at the end. The input to the network is a 32x32 pixel color image, which is 
classified into one of the 10 output classes. The model size is 91 KB.

Firstly a static ship image is used as input regardless camera is connected or not.
Secondly runtime image processing from camera in the case camera and lcd display
is connected. Camera data are displayed on LCD display. 

HOW TO USE THE APPLICATION:
To classify an image, place an image in front of the camera so that it fits in the
white rectangle in the middle of the LCD display. 
Note that semihosting implementation causes slower or discontinuous video experience. 
Select UART in 'Project Options' for using external debug console 
via UART (Virtual COM port).

[1] https://github.com/tensorflow/models/tree/master/research/slim
[2] https://github.com/tensorflow/tensorflow/tree/r1.14/tensorflow/lite/examples/label_image

Files:
  cifar10.cpp - example source code
  cifar10_model.h - model data converted from a .tflite file
  ship.bmp - shrinked picture of the object to recognize
    (source: https://en.wikipedia.org/wiki/File:Christian_Radich_aft_foto_Ulrich_Grun.jpg)
  ship.h - image file converted into a C language array of RGB values
    using Python with the OpenCV and Numpy packages:
    import cv2
    import numpy as np
    img = cv2.imread('ship.bmp')
    img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    with open('ship_image.h', 'w') as fout:
      print('#define SHIP_IMG_DATA {', file=fout)
      img.tofile(fout, ',', '%d')
      print('}\n', file=fout)
  get_top_n.h - get top result declarations
  get_top_n_impl - get top result soruce code
  timer.c - timer function source code
  timer.h - timer function declarations
  image.c - image processing source code
  image.h - image processing function declarations


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
    - EVKB-IMXRT1050 board
    - Personal computer

- Camera input
    - Mini/micro USB cable
    - EVKB-IMXRT1050 board
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

CIFAR-10 object recognition example using a TensorFlow Lite model. 
Detection threshold: 60%

Static data processing:
----------------------------------------
     Inference time: 56 ms
     Detected: ship (100%)
----------------------------------------

Camera data processing:
----------------------------------------
     Inference time: 59 ms
     Detected: cat (77%)
----------------------------------------

----------------------------------------
     Inference time: 58 ms
     Detected: ship (98%)
----------------------------------------

----------------------------------------
     Inference time: 58 ms
     Detected: cat (67%)
----------------------------------------

----------------------------------------
     Inference time: 57 ms
     Detected: airplane (76%)
----------------------------------------

----------------------------------------
     Inference time: 57 ms
     Detected: automobile (83%)
----------------------------------------

----------------------------------------
     Inference time: 58 ms
     Detected: frog (64%)
----------------------------------------
Customization options
=====================

