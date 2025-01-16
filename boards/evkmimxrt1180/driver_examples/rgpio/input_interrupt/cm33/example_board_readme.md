Hardware requirements
=====================
- Micro USB cable
- MIMXRT1180-EVK board
- Personal Computer

Board settings
============
No special settings are required.

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
================
When the example runs successfully, if you press the SW8, the "SW8 is pressed" will show in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 GPIO Driver example

 Press SW8
 SW8 is pressed
 SW8 is pressed
 SW8 is pressed
 ...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
