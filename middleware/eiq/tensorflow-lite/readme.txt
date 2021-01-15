eIQ TensorFlow Lite library 2.3.1

Content
-------
1. Introduction
3. Directory structure
4. eIQ TensorFlow Lite examples
5. Documentation
6. Library configuration
7. Release notes
8. Limitations

1. Introduction
---------------
TensorFlow Lite is an open source software library for running machine learning
models on mobile and embedded devices. It enables on-device machine learning
inference. Models trained by TensorFlow can be converted into a compressed
FlatBuffers format with the TensorFlow Lite Converter (available in
the TensorFlow distributions). Converted .tflite files can be then downloaded
into an embedded device. The eIQ TensorFlow Lite library is based on
TensorFlow Lite 2.3.1 and is therefore fully compatible. 
Note: TensorFlow Lite supports only a subset of operators available in
      TensorFlow. The conversion tool reports any unsupported operators during
      the conversion.

2. Directory structure
--------------------------------------
<MCUXpresso-SDK-root>
|-- boards
|   -- <board>
|      -- eiq_examples                         - Example build projects
|         -- tensorflow_lite_adt               - Anomaly detection example
|         -- tensorflow_lite_benchmark         - Benchmark tool
|         -- tensorflow_lite_cifar10           - CIFAR-10 image recognition example
|         -- tensorflow_lite_kws               - Keyword spotting example
|         -- tensorflow_lite_label_image       - Image recognition example
|         -- tensorflow_lite_micro_label_image - Image recognition example
|                                                using TensorFlow Lite Micro
|         -- tensorflow_lite_lib               - Library build project
|-- middleware
    -- eiq
       -- tensorflow-lite
          -- examples                  - Example source codes
          -- lib                       - Library binaries
          -- tensorflow                - Library source codes
          -- third_party               - 3rd party library source codes

3. eIQ TensorFlow Lite examples
------------------------------------
The package contains several example applications using
the eIQ TensorFlow Lite library. The build projects can be found in
the /boards/<board>/eiq_examples/tensorflow_lite_* folders.
The following boards are currently supported: evkbimxrt1050, evkmimxrt1060.
The following toolchains are currently supported:
MCUXpresso IDE, IAR Embedded Workbench, Keil MDK, GNU ARM Embedded Toolchain

4. Documentation
----------------
4.1. The TensorFlow Lite software library documentation is available at
     http://www.tensorflow.org/lite/.
4.2. How to convert models from the TensorFlow format is described at
     https://www.tensorflow.org/lite/convert/.

5. Library configuration
------------------------
5.1. Stack memory configuration
     During the library compilation, based on the stack memory configuration,
     the EIGEN_STACK_ALLOCATION_LIMIT macro definition can be set to the maximum
     size of temporary objects that can be allocated on the stack
     (they will be dynamically allocated instead). A high number may cause stack
     overflow. A low number may decrease object allocation performance.

6. Release notes
----------------
The library is based on TensorFlow Lite version 2.3.1 available
at https://github.com/tensorflow/tensorflow/tree/v2.3.1/tensorflow/lite.
Main modifications introduced to the original library source code:
  * Removed files not containing the TensorFlow Lite library source codes or
    not needed for building the TensorFlow Lite library
  * Replaced multi-threaded execution with single threaded on MCUs
  * Fixed source code to build with MCUXpresso SDK supported toolchains
  * Added third party library source codes
    * Abseil (https://github.com/abseil/abseil-cpp/archive/df3ea785d8c30a9503321a3d35ee7d35808f190d.tar.gz)
      * Removed files not needed for compiling the TensorFlow Lite library
    * CMSIS-NN (https://github.com/ARM-software/CMSIS_5/archive/1150e71e07c79b538efd842aba5b210a31827ae5.zip)
      * Removed files not needed for compiling the TensorFlow Lite library
    * Eigen (https://gitlab.com/libeigen/eigen/-/archive/386d809bde475c65b7940f290efe80e6a05878c4/eigen-386d809bde475c65b7940f290efe80e6a05878c4.tar.gz)
      * Removed files not needed for compiling the TensorFlow Lite library
      * Added support for the IAR Embedded Workbench compiler
    * Farmhash (http://mirror.tensorflow.org/github.com/google/farmhash/archive/816a4ae622e964763ca0862d9dbd19324a1eaf45.tar.gz)
      * Removed files not needed for compiling the TensorFlow Lite library
      * Added support for the IAR Embedded Workbench compiler
    * FFT2D (http://mirror.tensorflow.org/www.kurims.kyoto-u.ac.jp/~ooura/fft.tgz)
      * Removed files not needed for compiling the TensorFlow Lite library
    * FlatBuffers (https://github.com/google/flatbuffers/archive/v1.12.0.tar.gz)
      * Removed files not needed for compiling the TensorFlow Lite library
      * Added support for the IAR Embedded Workbench compiler
    * Gemmlowp (https://github.com/google/gemmlowp/archive/fda83bdc38b118cc6b56753bd540caa49e570745.zip)
      * Removed files not needed for compiling the TensorFlow Lite library
      * Added support for the IAR Embedded Workbench compiler
      * Added support for single threaded execution on MCUs
    * Ruy (https://github.com/google/ruy/archive/34ea9f4993955fa1ff4eb58e504421806b7f2e8f.zip)
      * Removed files not needed for compiling the TensorFlow Lite library
      * Added support for the IAR Embedded Workbench compiler
      * Added support for single threaded execution on MCUs
  * Added build projects for MCUXpresso SDK supported toolchains

7. Limitations
--------------
* Profiling using the Gemmlowp library is not supported on MCUs
