Overview
========
The GPIO Example project is a demonstration program that uses the KSDK software to manipulate the general-purpose
outputs. The example use LEDs and buttons to demonstrates GPIO API for port and pin manipulation (init, set,
clear, and toggle).



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
1. Connect a micro USB cable between the PC host and the CMSIS DAP USB port (J1) on the board
2. Open a serial terminal with the following settings (See Appendix A in Getting started guide for description how to determine serial port number):
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3. Download the program to the target board.


Running the demo
================
After the board is flashed the Tera Term will print message on terminal.

Example output:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 GPIO Driver example

 The LED is taking turns to shine.

 Standard port read: 40290

 Masked port read: 40000

 Port state: 64001804

 Port state: 64001804

 Port state: 64001804

 Port state: 64001804
 ...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~
- When press sw1 the LED GREEN will turn to shine 
