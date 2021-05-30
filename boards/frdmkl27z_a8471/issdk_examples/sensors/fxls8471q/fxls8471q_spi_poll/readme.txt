Hardware requirements
===================
- Mini/micro USB cable
- FRDMKL27Z board
- FRDM-STBC-A8471 board
- Personal Computer

Demo Overview
============
The FXLS8741Q Normal example application demonstrates the use of the FXLS8741Q
sensor in the normal mode.
The example demonstrates configuration of all registers required to put the sensor
into normal mode and read out samples.

The application constantly checks for the data ready flag and read the samples when its ready.

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
