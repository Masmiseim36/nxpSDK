eIQ TensorFlow Lite for Microcontrollers library 2.4.1

Content
-------
1. Introduction
3. Directory structure
4. eIQ TensorFlow Lite examples
5. Documentation
6. Release notes
7. Limitations

1. Introduction
---------------
TensorFlow Lite is an open source software library for running machine learning
models on mobile and embedded devices. It enables on-device machine learning
inference. Models trained by TensorFlow can be converted into a binary
FlatBuffers format with the TensorFlow Lite Converter (available in
the TensorFlow distributions). Converted .tflite files can be then downloaded
into an embedded device. The eIQ TensorFlow Lite for Microcontrollers library
is based on TensorFlow Lite for Microcontrollers 2.4.1 and is therefore
fully compatible.
Note: TensorFlow Lite supports only a subset of operators available in
      TensorFlow. The conversion tool reports any unsupported operators during
      the conversion.

2. Directory structure
----------------------
<MCUXpresso-SDK-root>
|-- boards
|   -- <board>
|      -- eiq_examples                         - Example build projects
|         -- tensorflow_lite_micro_adt         - Anomaly detection example
|         -- tensorflow_lite_micro_cifar10     - CIFAR-10 image recognition example
|         -- tensorflow_lite_micro_kws         - Keyword spotting example
|         -- tensorflow_lite_micro_label_image - Image recognition example
|-- middleware
    -- eiq
       -- tensorflow-lite
          -- examples                  - Example source codes
          -- tensorflow                - Library source codes
          -- third_party               - 3rd party library source codes

3. eIQ TensorFlow Lite examples
-------------------------------
The package contains several example applications using
the eIQ TensorFlow Lite for Microcontrollers library. The build projects can
be found in the /boards/<board>/eiq_examples/tensorflow_lite_micro_* folders.
The following boards are currently supported: evkbimxrt1050, evkmimxrt1060,
evkbimxrt1060, evkmimxrt1064, evkmimxrt1160, evkmimxrt1170, evkmimxrt595,
evkmimxrt685.
The following toolchains are currently supported:
MCUXpresso IDE, IAR Embedded Workbench, Keil MDK, GNU ARM Embedded Toolchain

4. Documentation
----------------
4.1. The TensorFlow Lite for Microcontrollers software library documentation
     is available at https://www.tensorflow.org/lite/microcontrollers.
4.2. How to convert models from the TensorFlow format is described at
     https://www.tensorflow.org/lite/convert/.

5. Optimizations
----------------
List of hardware optimized operators:

Operator         |Operator  |i.MX RT685  |i.MX RT595     |i.MX RT1170
                 |input type|(HiFi4 core)|(FusionF1 core)|i.MX RT1160
                 |          |            |               |i.MX RT1064
                 |          |            |               |i.MX RT1060
                 |          |            |               |i.MX RT1050
                 |          |            |               |i.MX RT685 (Cortex-M33 core)
                 |          |            |               |i.MX RT595 (Cortex-M33 core)
-----------------+----------+------------+---------------+----------------------------
ADD              |FP        |Yes         |No             |No
                 |PTQ       |Yes         |Yes            |No
                 |PCQ       |No          |No             |Yes
-----------------+----------+------------+---------------+----------------------------
AVERAGE_POOL_2D  |FP        |Yes         |No             |No
                 |PTQ       |Yes         |Yes            |No
                 |PCQ       |No          |No             |Yes
-----------------+----------+------------+---------------+----------------------------
CONV_2D          |FP        |Yes         |No             |No
                 |PTQ       |Yes         |Yes            |No
                 |PCQ       |No          |No             |Yes
-----------------+----------+------------+---------------+----------------------------
DEPTHWISE_CONV_2D|FP        |Yes         |No             |No
                 |PTQ       |Yes         |Yes            |Yes
                 |PCQ       |No          |No             |Yes
-----------------+----------+------------+---------------+----------------------------
FLOOR            |FP        |Yes         |No             |No
                 |PTQ       |N/A         |N/A            |N/A
                 |PCQ       |N/A         |N/A            |N/A
-----------------+----------+------------+---------------+----------------------------
FULLY_CONNECTED  |FP        |Yes         |No             |No
                 |PTQ       |Yes         |Yes            |No
                 |PCQ       |No          |No             |Yes
-----------------+----------+------------+---------------+----------------------------
LOGISTIC         |FP        |Yes         |No             |No
                 |PTQ       |No          |No             |No
                 |PCQ       |N/A         |N/A            |N/A
-----------------+----------+------------+---------------+----------------------------
MAX_POOL_2D      |FP        |Yes         |No             |No
                 |PTQ       |Yes         |Yes            |No
                 |PCQ       |No          |No             |Yes
-----------------+----------+------------+---------------+----------------------------
MUL              |FP        |Yes         |No             |No
                 |PTQ       |Yes         |Yes            |No
                 |PCQ       |No          |No             |Yes
-----------------+----------+------------+---------------+----------------------------
RELU             |FP        |Yes         |No             |No
                 |PTQ       |Yes         |Yes            |No
                 |PCQ       |No          |No             |No
-----------------+----------+------------+---------------+----------------------------
RELU6            |FP        |Yes         |No             |No
                 |PTQ       |Yes         |Yes            |No
                 |PCQ       |No          |No             |No
-----------------+----------+------------+---------------+----------------------------
SOFTMAX          |FP        |Yes         |No             |No
                 |PTQ       |Yes         |Yes            |No
                 |PCQ       |No          |No             |Yes
-----------------+----------+------------+---------------+----------------------------
SVDF             |FP        |Yes         |No             |No
                 |PTQ       |N/A         |N/A            |N/A
                 |PCQ       |No          |No             |Yes

FP - 32-bit floating point
PTQ - per-tensor quantized (asymmetric 8-bit quantization)
PCQ - per-channel quantized (symmetric 8-bit quantization)

Operators not listed here are reference C++ implementations only.
Optimized kernels for ARM Cortex-M cores leverage the ARM CMSIS-NN library.
Optimized kernels for Cadence Xtensa cores leverage the Xtensa HiFi4 NN library.

6. Release notes
----------------
The library is based on TensorFlow Lite for Microcontrollers (TFLM) version 2.4.1
available at https://github.com/tensorflow/tensorflow/tree/v2.4.1/tensorflow/lite/micro.
Main modifications introduced to the original library source code:
  * Removed files not containing the TFLM library source codes
    or not needed for building the TFLM library
  * Added third party library source codes
    * CMSIS-NN (https://github.com/ARM-software/CMSIS_5/archive/01f5b32badf7b78c85a24a7149b56400fa6a2999.zip)
      * Removed files not needed for compiling the TFLM library
    * Xtensa HiFi4 NN (https://github.com/foss-xtensa/nnlib-hifi4/blob/master/archive/xa_nnlib_06_27.zip)
    * FFT2D (http://mirror.tensorflow.org/www.kurims.kyoto-u.ac.jp/~ooura/fft.tgz)
      * Removed files not needed for compiling the TFLM library
    * FlatBuffers (https://github.com/google/flatbuffers/archive/dca12522a9f9e37f126ab925fd385c807ab4f84e.zip)
      * Removed files not needed for compiling the TFLM library
    * Gemmlowp (https://github.com/google/gemmlowp/archive/719139ce755a0f31cbf1c37f7f98adcc7fc9f425.zip)
      * Removed files not needed for compiling the TFLM library
    * Ruy (https://github.com/google/ruy/archive/5bb02fbf90824c2eb6cd7418f766c593106a332b.zip)
      * Removed files not needed for compiling the TFLM library
  * Added build projects for MCUXpresso SDK supported toolchains

7. Limitations
--------------
* No known limitations
