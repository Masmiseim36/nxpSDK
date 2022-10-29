Hardware requirements
===================
- Mini/micro USB cable
- FRDM-K22F board
- FRDM-STBI-A8974 board
- Personal Computer

Board settings
==============
On FRDM-STBI-A8974 sensor shield board:
--------------------------------------
Since the examples use I2C1, Pins 2-3 of SW2 on FRDM-STBI-A8974 should be connected.
SW1 Pins 2-3 should be connected to select default operating mode i.e. "ACCEL NORMAL" mode.

Demo Overview
============
The example demonstrates configuration of all registers required to use the fxls896x sensor as the
acceleration source for the pedometer. Pedometer algorithm measures step counts based on fxls896x
sensor data.

Prepare the Demo
===============
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
===============
When the demo runs successfully, you can see the step counts printed to the terminal.

Note: Walk with the board to get the step counts to change.
