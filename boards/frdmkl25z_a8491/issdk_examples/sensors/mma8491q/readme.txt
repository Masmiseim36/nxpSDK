Overview
========
The MMA8491Q example application demonstrates the use of the MMA8491Q sensor.
The example demonstrates how to put the sensor into Active Mode and read XYZ samples and Tilt output from the sensor.

The sensor reads samples as per the acquisition time and sets a Flag in the status register.
The application consistently checks the Flag after Turn On period and and when set reads out the sample.

Toolchain supported
===================
- IAR embedded Workbench 7.80.4
- Keil MDK 5.21a
- GCC ARM Embedded 2016-5.4-q3
- Kinetis Development Studio IDE 3.2.0
- MCUXpresso0.8

Hardware requirements
=====================
- Mini/micro USB cable
- FRDMKL25Z board
- FRDM-STBC-A8491 board
- Personal Computer

Board settings
==============

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the demo runs successfully, you can see the Samples printed to the terminal.

Note: you must rotate the board to get the samples to change.
Customization options
=====================

