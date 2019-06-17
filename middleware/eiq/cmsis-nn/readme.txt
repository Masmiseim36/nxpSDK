eIQ CMSIS-NN library 1.0

Content
-------
1. Introduction
2. Directory structure
3. eIQ CMSIS-NN examples
4. Documentation
5. Release Notes

1. Introduction
---------------
The CMSIS-NN software library is a collection of efficient neural network
kernels developed to maximize the performance and minimize the memory footprint
of neural networks on Cortex-M processor cores
(http://www.keil.com/pack/doc/CMSIS_Dev/NN/html/index.html).

2. CMSIS-NN directory structure
-------------------------------
<MCUXpresso-SDK-root>
|-- boards
|   -- <board>
|      -- eiq_examples                 - Example build projects
|         -- cmsis_nn_cifar10          - CIFAR-10 image recognition example
|         -- cmsis_nn_kws              - Keyword spotting example
|-- middleware
    -- eiq
       -- cmsis-nn
          -- Examples                  - Example source codes
          -- Include                   - Library include headers
          -- Source                    - Library source files

3. eIQ CMSIS-NN examples
-----------------------------
The package contains several example applications using the CMSIS-NN library.
The build projects can be found in the /boards/<board>/eiq_examples/cmsis_nn_*
folders.
The following boards are currently supported: evkbimxrt1050, evkmimxrt1060
The following toolchains are currently supported:
MCUXpresso IDE, IAR Embedded Workbench, Keil MDK, GNU ARM Embedded Toolchain

4. Documentation
----------------
4.1. The CMSIS NN software library documentation is available at
     http://www.keil.com/pack/doc/CMSIS_Dev/NN/html/index.html.
4.2. How to train, optimize and convert models from Caffe format is described
     in an example at https://github.com/ARM-software/ML-examples/tree/master/cmsisnn-cifar10.

5. Release notes
----------------
The library is based on CMSIS-NN version 1.0.0 available
at https://github.com/ARM-software/CMSIS_5 as a part of CMSIS version 5.4.0.
Main modifications introduced to the original library source code:
  * Removed NN test source codes
  * Removed Python scripts
  * Removed example code for GRU
  * Added KWS example based on code from https://github.com/ARM-software/ML-KWS-for-MCU
  * Added build projects for MCUXpresso SDK supported toolchains
