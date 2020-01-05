Hardware requirements
===================
- Mini/micro USB cable
- FRDMKL27Z board
- Personal Computer

Demo Overview
============
The MAG3110 One-Shot example application demonstrates the use of the MAG3110
sensor in the One-Shot (on demand sampling) Mode. The example demonstrates
configuration of all registers reguired to put the sensor into One-Shot Mode
and read out a MagneticField sample.

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
