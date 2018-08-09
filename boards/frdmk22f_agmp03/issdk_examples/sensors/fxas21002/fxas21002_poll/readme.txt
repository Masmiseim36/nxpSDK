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
The FXAS21002 Normal example application demonstrates the use of the FXAS21002 sensor in the Poll (Non-Buffered) Mode.
The example demonstrates configuration of all registers reguired to put the sensor into Standard Mode and read out a sample.

The sensor reads samples as per the configured ODR and sets a Flag.
The application consistently checks the Flag and when set reads out the sample.s.

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
When the demo runs successfully, you can see the Samples printed to the terminal.

Note: you must rotate the board to get the samples to change.
