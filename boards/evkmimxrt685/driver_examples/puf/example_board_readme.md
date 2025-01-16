Hardware requirements
=====================
- Micro USB cable
- EVK-MIMXRT685 board
- Personal Computer

Board settings
============
No special settings are required.

Prepare the Demo
===============
1.  Connect a micro USB cable between the PC host and the CMSIS DAP USB port (J5) on the board
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

Reconstructed key = 1c 1 fc d5 ca 7f df 42 1f e4 85 bc 7e 19 c2 42
Example end.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
