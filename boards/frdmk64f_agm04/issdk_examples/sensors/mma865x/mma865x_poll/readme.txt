Hardware requirements
===================
- Mini/micro USB cable
- FRDMK64F board
- FRDM-STBC-AGM04 board
- Personal Computer

Board settings
==============
On FRDM-STBC-AGM04 sensor shield board:
---------------------------------------
For I2C Mode: The examples use I2C1 and Pins 2-3 of Jumpers J7 and J8 on FRDM-STBC-AGM04 should be connected.

Demo Overview
============
The MMA865x Poll example application demonstrates the use of the MMA865x sensor in the Poll (Non-Buffered) Mode.
The example demonstrates configuration of all registers reguired to put the sensor into Normal Mode and read out samples.

The sensor reads samples as per the configured ODR and sets a Flag.
The application consistently checks the Flag and when set reads out the sample.

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
