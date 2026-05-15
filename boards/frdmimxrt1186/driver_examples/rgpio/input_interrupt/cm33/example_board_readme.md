Hardware requirements
=====================
- Micro USB cable
- FRDM-IMXRT1186 board
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
When the example runs successfully, if you press the SW4, the "SW4 is pressed" will show in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 GPIO Driver example

 Press SW4
 SW4 is pressed
 SW4 is pressed
 SW4 is pressed
 ...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
