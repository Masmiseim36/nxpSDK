Overview
========

The dsp_nn_demo demo application demonstrates starting DSP core with DSP image and accelerating some neural networks layers using the DSP.


Toolchain supported
===================
- IAR embedded Workbench  8.50.1
- MCUXpresso  11.2.0
- Keil MDK  5.30
- GCC ARM Embedded  9.2.1

Hardware requirements
=====================
- Mini/micro USB cable
- JTAG/SWD
- evkmimxrt685 board
- Personal Computer

Board settings
==============
Prepare the Demo
================
1.  Connect a USB cable between the PC host and the OpenSDA USB port on the target board.
2.  Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program for CM33 core to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.
5.  Detach from board.
6.  Download the program for DSP core to the target board.
7.  Launch the debugger in your IDE to begin running the demo.

Running the demo CM33
=====================
When the demo runs successfully, the terminal will display the following:

Started NN UnitTests and benchmarks (10 iterations)
DSP Image copied to SRAM
Running ECHO SYNC
ECHO unit test succeded
NNlib version 2.1
Avg Inference cycles: 4390 time: 0.070 ms
Throughput: 14285.7 fps
Running CONV DS SYNC
CONV DS output check succeded
Avg Inference cycles: 1092492 time: 17.472 ms
Throughput: 57.2 fps
Running CONV DS ASYNC
CONV DS output check succeded
Avg Inference cycles: 1096907 time: 17.543 ms
Throughput: 57.0 fps
Running CONV STD SYNC
CONV STD output check succeded
Avg Inference cycles: 1750516 time: 27.996 ms
Throughput: 35.7 fps
Running CONV STD ASYNC
CONV STD output check succeded
Avg Inference cycles: 1752746 time: 28.032 ms
Throughput: 35.7 fps
Running RELU SYNC
RELU unit test succeded
Avg Inference cycles: 6049 time: 0.096 ms
Throughput: 10416.7 fps
Running RELU ASYNC
RELU unit test succeded
Avg Inference cycles: 8266 time: 0.132 ms
Throughput: 7575.8 fps
Running MAXPOOL SYNC
MAXPOOL output check succeded
Avg Inference cycles: 21018 time: 0.336 ms
Throughput: 2976.2 fps
Running MAXPOOL ASYNC
MAXPOOL output check succeded
Avg Inference cycles: 23248 time: 0.371 ms
Throughput: 2695.4 fps
Customization options
=====================

