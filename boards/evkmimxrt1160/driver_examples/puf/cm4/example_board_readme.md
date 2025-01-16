Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT1160-EVK board
- Personal Computer

Board settings
============
No special settings are required.

Prepare the Demo
===============
1.  Connect a micro USB cable between the PC host and the CMSIS DAP USB port (P6) on the board
2.  Open a serial terminal with the following settings (See Appendix A in Getting started guide for description how to determine serial port number):
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
The log below shows the output of the PUF driver example in the terminal window (the key value will vary):
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
PUF Peripheral Driver Example

Reconstructed key = ad e7 b4 fe 99 ce 41 b7 c6 f9 78 2f 49 6b 87 27
Example end.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Note: Please make sure PUF is enabled in eFUSE (OCOTP) memory