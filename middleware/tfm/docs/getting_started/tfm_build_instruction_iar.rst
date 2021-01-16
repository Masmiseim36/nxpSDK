###################################################
Additional build instructions for the IAR toolchain
###################################################

Follow the instructions in
:doc:`software requirements <tfm_build_instruction>`, but replace the -DCOMPILER= setting with IARARM.


Notes for building with IARARM
------------------------------

    IAR Embedded Workbench for ARM (EWARM) versions 8.42 or later are required.

    Currently the MUSCA_B1, MUSCA_S1 and SSE-200_AWS targets are not supported with IARARM,
    due to lack of testing. The FVP_SSE300_MPS2 target is currently not supported by IARARM.

    cmake needs to be version 3.14 or newer.

    The V8M IAR CMSIS_5 RTX libraries in CMSIS_5 5.5.0 has a problem and has been updated in
    CMSIS_5 5.7.0. The updated libraries are part of the tf-m-tests repo and no special instructions
    are needed when the libraries from this repo are used.

    For all configurations and build options some of the QCBOR tests fail due to the tests not handling
    double float NaN:s according to the Arm Runtime ABI. This should be sorted out in the future.

    Some minor changes to the mbed-crypto pack is required to allow building TF-M with the
    IAR tools. This will be sorted out once tf-m upgrades to >3.1.0

    For mbed-crypto (CMakeLists.txt):

.. code-block:: bash

     if(CMAKE_COMPILER_IS_IAR)
    -    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --warn_about_c_style_casts --warnings_are_errors -Ohz")
    +    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --warn_about_c_style_casts")
    +    set(CMAKE_C_FLAGS_RELEASE     "-Ohz")
    +    set(CMAKE_C_FLAGS_DEBUG       "--debug -On")
    +    set(CMAKE_C_FLAGS_CHECK       "--warnings_are_errors")
     endif(CMAKE_COMPILER_IS_IAR)


Build steps:
============
.. code-block:: bash

    cd <base folder>
    cd trusted-firmware-m
    mkdir cmake_build
    cd cmake_build
    cmake ../ -G"Unix Makefiles" -DTARGET_PLATFORM=AN521 -DCOMPILER=IARARM
    cmake --build ./ -- install

Regression Tests for the AN521 target platform
==============================================
*TF-M build regression tests on Linux*

.. code-block:: bash

    cd <base folder>
    cd trusted-firmware-m
    mkdir cmake_test
    cd cmake_test
    cmake -G"Unix Makefiles" -DPROJ_CONFIG=`readlink -f ../configs/ConfigRegression.cmake` -DTARGET_PLATFORM=AN521 -DCOMPILER=IARARM ../
    cmake --build ./ -- install

*TF-M build regression tests on Windows*

.. code-block:: bash

    cd <base folder>
    cd trusted-firmware-m
    mkdir cmake_test
    cd cmake_test
    cmake -G"Unix Makefiles" -DPROJ_CONFIG=`cygpath -am ../configs/ConfigRegression.cmake` -DTARGET_PLATFORM=AN521 -DCOMPILER=IARARM ../
    cmake --build ./ -- install

 *Copyright (c) 2020, Arm Limited. All rights reserved.*
