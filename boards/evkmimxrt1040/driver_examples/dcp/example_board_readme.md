Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT1040-EVK board
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
Example output on terminal:

DCP Driver Example

AES ECB Test pass
AES CBC Test pass
SHA-1 Test pass
SHA-256 Test pass
CRC Test pass

Note:
There will be no log printed when downloading by IAR or keil. This issue will be fixed in later release.