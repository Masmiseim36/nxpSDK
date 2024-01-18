eIQ TensorFlow Lite for Microcontrollers library 23-09-18

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
is based on TensorFlow Lite for Microcontrollers, state from
the 23rd of March 2023, and is therefore fully compatible.
Note: TensorFlow Lite supports only a subset of operators available in
      TensorFlow. The conversion tool reports any unsupported operators during
      the conversion.

2. Directory structure
----------------------
<MCUXpresso-SDK-root>
|-- boards
|   -- <board>
|      -- eiq_examples        - Example build projects
|         -- tflm_cifar10     - CIFAR-10 image recognition example
|         -- tflm_kws         - Keyword spotting example
|         -- tflm_label_image - Image recognition example
|-- middleware
    -- eiq
       -- tensorflow-lite
          -- examples         - Example source codes
          -- lib              - Library binaries
          -- tensorflow       - Library source codes
          -- third_party      - 3rd party library source codes

3. eIQ TensorFlow Lite examples
-------------------------------
The package contains several example applications using
the eIQ TensorFlow Lite for Microcontrollers library. The build projects can
be found in the /boards/<board>/eiq_examples/tflm_* folders.
The following boards are currently supported: evkmimxrt1040, evkbimxrt1050,
evkmimxrt1060, evkbimxrt1060, evkmimxrt1064, evkmimxrt1160, evkmimxrt1170,
evkmimxrt595, evkmimxrt685, mimxrt685audevk, MCX-N947-EVK, MCX-N947-BRK,
MCX-N548-EVK.
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

Operator                    |Operator  |i.MX RT595 (FusionF1 core)|i.MX RT1040                 |MCX-N947 (NPU)
                            |input type|i.MX RT685 (HiFi4 core)   |i.MX RT1050                 |MCX-N548 (NPU)
                            |          |                          |i.MX RT1060                 |
                            |          |                          |i.MX RT1064                 |
                            |          |                          |i.MX RT1160                 |
                            |          |                          |i.MX RT1170                 |
                            |          |                          |i.MX RT595 (Cortex-M33 core)|
                            |          |                          |i.MX RT685 (Cortex-M33 core)|
----------------------------+----------+--------------------------+----------------------------+--------------
ADD                         |FP        |No                        |No                          |No
                            |PTQ       |N/A                       |N/A                         |No
                            |PCQ       |No                        |Yes                         |Yes
----------------------------+----------+--------------------------+----------------------------+--------------
AVERAGE_POOL_2D             |FP        |No                        |No                          |No
                            |PTQ       |N/A                       |N/A                         |No
                            |PCQ       |No                        |Yes                         |Yes
----------------------------+----------+--------------------------+----------------------------+--------------
CONV_2D                     |FP        |N/A                       |No                          |No
                            |PTQ       |N/A                       |N/A                         |No
                            |PCQ       |Yes                       |Yes                         |Yes
----------------------------+----------+--------------------------+----------------------------+--------------
DEPTHWISE_CONV_2D           |FP        |N/A                       |No                          |No
                            |PTQ       |N/A                       |N/A                         |No
                            |PCQ       |Yes                       |Yes                         |Yes
----------------------------+----------+--------------------------+----------------------------+--------------
FULLY_CONNECTED             |FP        |N/A                       |No                          |No
                            |PTQ       |N/A                       |N/A                         |No
                            |PCQ       |Yes                       |Yes                         |Yes
----------------------------+----------+--------------------------+----------------------------+--------------
UNIDIRECTIONAL_SEQUENCE_LSTM|FP        |N/A                       |No                          |No
                            |PTQ       |N/A                       |N/A                         |No
                            |PCQ       |Yes                       |No                          |No
----------------------------+----------+--------------------------+----------------------------+--------------
LOGISTIC                    |FP        |Yes (on i.MX RT685)       |No                          |No
                            |PTQ       |N/A                       |N/A                         |No
                            |PCQ       |Yes                       |No                          |No
----------------------------+----------+--------------------------+----------------------------+--------------
MAX_POOL_2D                 |FP        |No                        |No                          |No
                            |PTQ       |N/A                       |N/A                         |No
                            |PCQ       |No                        |Yes                         |Yes
----------------------------+----------+--------------------------+----------------------------+--------------
MUL                         |FP        |No                        |No                          |No
                            |PTQ       |No                        |N/A                         |No
                            |PCQ       |No                        |Yes                         |No
----------------------------+----------+--------------------------+----------------------------+--------------
SOFTMAX                     |FP        |No                        |No                          |No
                            |PTQ       |N/A                       |N/A                         |No
                            |PCQ       |Yes                       |Yes                         |No
----------------------------+----------+--------------------------+----------------------------+--------------
SVDF                        |FP        |No                        |No                          |No
                            |PTQ       |N/A                       |N/A                         |No
                            |PCQ       |Yes                       |Yes                         |No

FP - 32-bit floating point
PTQ - per-tensor quantized (asymmetric 8-bit quantization)
PCQ - per-channel quantized (symmetric 8-bit quantization)

Operators not listed here are reference C++ implementations only.
Optimized kernels for ARM Cortex-M cores leverage the ARM CMSIS-NN library
(for more details see third_party/cmsis/CMSIS/NN/README.md)
Optimized kernels for Cadence Xtensa cores leverage the Xtensa HiFi4 NN library.
The optimized custom operator for NXP eIQ Neutron NPU leverage the eIQ Neutron NPU driver and firmware.

6. Release notes
----------------
The library is based on TensorFlow Lite for Microcontrollers (TFLM) from
the 18th of September 2023 available at https://github.com/tensorflow/tflite-micro/commit/127f88fe3aaed550c6d7f644c07c8f407a45cf0f.
Main modifications introduced to the original library source code:
  * Removed files not containing the TFLM library source codes
    or not needed for building the TFLM library
  * Added third party library source codes
    * CMSIS-NN (http://github.com/ARM-software/CMSIS-NN/archive/dc64e488f6655aa2792d2aceca316c896f78b4db.zip)
      * Removed files not needed for compiling the TFLM library
    * Xtensa HiFi4 NN (http://github.com/foss-xtensa/nnlib-hifi4/raw/master/archive/xa_nnlib_hifi4_10_14_2022.zip)
    * FFT2D (http://mirror.tensorflow.org/www.kurims.kyoto-u.ac.jp/~ooura/fft.tgz)
      * Removed files not needed for compiling the TFLM library
    * KissFFT (https://github.com/mborgerding/kissfft/archive/refs/tags/v130.zip)
      * Removed files not needed for compiling the TFLM library
    * FlatBuffers (https://github.com/google/flatbuffers/archive/a66de58af9565586832c276fbb4251fc416bf07f.zip)
      * Removed files not needed for compiling the TFLM library
    * Gemmlowp (https://github.com/google/gemmlowp/archive/719139ce755a0f31cbf1c37f7f98adcc7fc9f425.zip)
      * Removed files not needed for compiling the TFLM library
    * Ruy (https://github.com/google/ruy/archive/d37128311b445e758136b8602d1bbd2a755e115d.zip)
      * Removed files not needed for compiling the TFLM library
  * Added build projects for MCUXpresso SDK supported toolchains

7. Limitations
--------------
* No known limitations
