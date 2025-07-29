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
by the following command. The minimum size of SRAM scratch memory will be displayed.
./neutron-converter --input mobilenet_v1_1.0_224_int8.tflite --output mobilenet_v1_1.0_224_int8_npu.tflite -target imxrt700 --fetch_constants_to_sram ture
Converting model with the following options:
  Input  = mobilenet_v1_1.0_224_int8.tflite
  Output = mobilenet_v1_1.0_224_int8_npu.tflite
  Target = imxrt700
Minimum size of SRAM scratch memory = 1082368 (bytes)

## Running the demo
The log below shows the output of the demo in the terminal window (compiled with ARM GCC):

Label image example using a TensorFlow Lite Micro model.
Detection threshold: 23%
Model: mobilenet_v1_1.0_224_int8_npu
Core/NPU Frequency: 324 MHz
TensorArena Addr: 0x20102020 - 0x20282020
TensorArena Size: Total 0x180000 (1572864 B); Used 0x14b284 (1356420 B)
Model Addr: 0x28200000 - 0x28625a70
Model Size: 0x425a70 (4348528 B)
Total Size Used: 5704948 B (Model (4348528 B) + TensorArena (1356420 B))

Static data processing:
----------------------------------------
     Inference time: 53376 us
     Detected: military uniform (88%)
----------------------------------------

Camera data processing:
Camera input is currently not supported on this device

## How to change for user’s larger model
The internal SRAM memory for scratching weight is scratchWeightsBuffer[SCRATCH_WEIGHTS_SRAM_SIZE].
It is from the start address of Non-cacheable memory (0x20400000 in this example).
SCRATCH_WEIGHTS_SRAM_SIZE is 1200KB in this example.

In the linker file, the m_data and m_ncache sections are configured as below.

m_data		(RW)  : ORIGIN = 0x20100000, LENGTH = 0x00300000
m_ncache	(RW)  : ORIGIN = 0x20400000, LENGTH = 0x00140000

When user runs a larger model, first check the minimum size of SRAM scratch memory as above.
Make sure SCRATCH_WEIGHTS_SRAM_SIZE > the minimum size of SRAM scratch memory and the total length
of m_ncache memory in the linker script is large enough to fit SCRATCH_WEIGHTS_SRAM_SIZE.

If more SRAM scratch memory is needed, you can do the following changes.

For example, an additional 1MB is needed for SRAM scratch memory.
1) Adjust m_data and m_ncache sections ORIGIN and LENGTH as needed.

m_text		(RX)  : ORIGIN = 0x28004300, LENGTH = 0x002FBD00
m_data		(RW)  : ORIGIN = 0x20000000, LENGTH = 0x00300000
m_ncache	(RW)  : ORIGIN = 0x20300000, LENGTH = 0x00240000
m_model		(RW)  : ORIGIN = 0x28300000, LENGTH = 0x00A00000

2) Increase SCRATCH_WEIGHTS_SRAM_SIZE to 2224KB.

static const unsigned int SCRATCH_WEIGHTS_SRAM_SIZE = 2224 * 1024

3) Change tensor arena size as needed.
const int  kTensorArenaSize = 2048 * 1024;

## Supported Boards
- [MIMXRT700-EVK](../../_boards/mimxrt700evk/eiq_examples/tflm_label_image_ext_mem/example_board_readme.md)
