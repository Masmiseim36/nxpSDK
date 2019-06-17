Overview
========
The DCP Example project is a demonstration program that uses the KSDK software implement
several cryptography algorithms using the DCP software driver.

Toolchain supported
===================
- GCC ARM Embedded  8.2.1
- IAR embedded Workbench  8.32.3
- Keil MDK  5.27
- MCUXpresso  11.0.0

Hardware requirements
=====================
- Mini/micro USB cable
- EVK-MIMXRT1064 board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
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
CRC-32 Test pass
Customization options
=====================

