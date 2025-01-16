Hardware requirements
=====================
- Mini/micro USB cable
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
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
When the demo runs successfully, will get the similar messages on the terminal.

~~~~~~~~~~~~~~~~~~~~~~
Starting System Counter ...
 System Counter compare interrupt is occurred !
 System Counter upper 24 bits is 0
 System Counter lower 32 bits is 44aa20a
 System Counter compare interrupt is occurred !
 System Counter upper 24 bits is 0
 System Counter lower 32 bits is 7270e0a
~~~~~~~~~~~~~~~~~~~~~~
