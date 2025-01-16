# tflm_label_image_ext_mem

## Overview
TensorFlow Lite model based implementation of object detector based on
TensorFlow Lite example [2] adjusted to run on MCUs.

The example uses mobilenet_v1_1.0 quantized model with 224x224 resolution to
demonstrate how to fetch model's weight from external memory(xSPI flash) to
internal SRAM for Neutron NPU execution.

A 3-channel color image is set as an input to a quantized Mobilenet
convolutional neural network model [1] that classifies the input image into
one of 1000 output classes.

Firstly a static image is set as input regardless camera is connected or not.
Secondly runtime image processing from camera in the case camera and display
is connected. Camera data are displayed on LCD.

HOW TO USE THE APPLICATION:
To classify an image, place an image in front of camera so that it fits in the
white rectangle in the middle of the display.
Note semihosting implementation causes slower or discontinuous video experience. 
Select UART in 'Project Options' during project import for using external debug console 
via UART (virtual COM port).

[1] https://www.tensorflow.org/lite/models
[2] https://github.com/tensorflow/tensorflow/tree/r2.3/tensorflow/lite/examples/label_image

The converted TensorFlow Lite Micro model mobilenet_v1_1.0_224_int8_npu.tflite is generated
by the following command.
./neutron-converter --input mobilenet_v1_1.0_224_int8.tflite --output mobilenet_v1_1.0_224_int8_npu.tflite -target imxrt700 --fetch_constants_to_sram ture

## Running the demo
The log below shows the output of the demo in the terminal window (compiled with ARM GCC):

Label image example using a TensorFlow Lite Micro model.
Detection threshold: 23%
Model: mobilenet_v1_1.0_224_int8_npu

Static data processing:
----------------------------------------
     Inference time: 69769 us 
     Detected: military uniform (88%)
----------------------------------------
Camera data processing:
Camera input is currently not supported on this device

## Supported Boards
- [MIMXRT700-EVK](../../_boards/mimxrt700evk/eiq_examples/tflm_label_image_ext_mem/example_board_readme.md)
