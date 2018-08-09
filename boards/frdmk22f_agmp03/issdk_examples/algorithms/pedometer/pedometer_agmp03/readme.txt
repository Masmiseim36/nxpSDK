Hardware requirements
===================
- Mini/micro USB cable
- FRDM-K22F board
- FRDM-STBC-AGMP03 board
- Personal Computer

Board settings
==============
On FRDM-STBC-AGMP03 sensor shield board:
--------------------------------------
For I2C Mode: The examples use I2C1 and Pins 2-3 of Jumpers J6 and J7 on FRDM-STBC-AGMP03 should be connected.
SW2 should be switched to "ACCEL NORMAL" mode and SW3 should be switched to "ACCEL-GYRO-I2C" mode.

Demo Overview
============
The example demonstrates configuration of all registers required to use the fxls8962 sensor as the
acceleration source for the pedometer. fxls8962 sensor detects motion and trigger step counts.

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

Note: you must walk with the board to get the step counts to change.
