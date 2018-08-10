Hardware requirements
===================
- Mini/micro USB cable
- FRDMK64F board
- FRDM-FXS-MULT2-B board
- Personal Computer

Board settings
==============
On FRDM-FXS-MULT2-B sensor shield board:
---------------------------------------
For I2C Mode: The examples use I2C1 and Pins 2-3 of Jumpers J27 and J28 on FRDM-FXS-MULT2-B should be connected.

Demo Overview
============
The MMA865x Freefall example application demonstrates the use of the MMA865x sensor to detect freefall.
The example demonstrates configuration of all registers reguired (including FF_MT) to put the sensor into Freefall detection mode and read detection events.

When the sensor detects freefall it sets a Flag.
The application consistently checks the Flag and when set, displays a message stating Freefall has been detected.

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
