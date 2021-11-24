Overview
========

The Hashcrypt Example project is a demonstration program that uses the KSDK software to encrypt plain text
and decrypt it back using AES and SHA algorithm. SHA-1, SHA-256, AES ECB and AES CBC modes are demonstrated.

Toolchain supported
===================
- IAR embedded Workbench  9.10.2
- Keil MDK  5.34
- GCC ARM Embedded  10.2.1
- MCUXpresso  11.4.0

Hardware requirements
=====================
- Mini/micro USB cable
- LPCXpresso55S06 board
- Personal Computer

Board settings
==============

Prepare the Demo
================
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
After the board is flashed the Tera Term will print message on terminal.

Example output:

```
AES ECB Test pass
AES CBC Test pass
AES CTR Test pass
SHA-1 Test pass
SHA-256 Test pass
```
