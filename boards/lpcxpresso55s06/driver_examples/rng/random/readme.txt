Overview
========

The RNG example project is a demonstration program that uses the KSDK software to generate random numbers
and prints them to the terminal. 32 32-bit numbers are read and skipped per one 32-bit number read and used.
This is to get a better entropy as suggested by user manual.

Toolchain supported
===================
- IAR embedded Workbench  9.10.2
- Keil MDK  5.34
- GCC ARM Embedded  10.2.1
- MCUXpresso  11.4.0

Hardware requirements
=====================
- Two micro USB cables
- LPCXpresso55S06 board
- Personal Computer

Board settings
==============

Prepare the Demo
================
1.  Connect a micro USB cable between the host PC and the +5V Power only USB port on the target board (J2).
2.  Connect a micro USB cable between the host PC and the Debug Link USB port on the target board (J1).
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
The log below shows example output of the random number generator demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Generate 128-bit random number:
0x868516634E5D992656DA94C7BFAD956A
 Press any key to continue...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
