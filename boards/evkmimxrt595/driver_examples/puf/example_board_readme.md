Hardware requirements
=====================
- Micro USB cable
- EVK-MIMXRT595 board
- Personal Computer

Board settings
============


Prepare the Demo
===============
1.  Connect a micro USB cable between the PC host and the CMSIS DAP USB port (J40) on the board
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Launch the debugger in your IDE to begin running the demo.

Running the demo
================
The log below shows the output of the PUF driver example in the terminal window (the key value will vary):
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
PUF Peripheral Driver Example

Reconstructed intrinsic key = 5a 66 b8 11 41 2e 74 4f fb 33 46 80 9c 9f 5c 1a 

Example end.
