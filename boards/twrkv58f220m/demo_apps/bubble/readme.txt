Overview
========


Toolchain supported
===================
- Keil MDK 5.24a
- IAR embedded Workbench 8.22.2
- GCC ARM Embedded 7-2017-q4-major
- MCUXpresso10.2.0

Hardware requirements
=====================
- Mini/Micro USB cable
- TWR-KV58 board
- Personal Computer

Board settings
==============
No special configure is needed.

Prepare the Demo
================
1.  Connect a mini/micro USB cable between the PC host and the OpenSDA USB port on the board.
2.  Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Unplug the usb cable about 10 second then plug again to begin running the demo.

Running the demo
================

indicates X-axis angle while another indicates Y-axis angle.
  When the example runs successfully, you can see the similar information from the terminal as below.


Welcome to the BUBBLE example

You will see angle data change in the console when change the angles of board

x=  6 y = 22
x=  8 y = 26
x= 10 y = 28
x= 10 y = 28
x= 11 y = 29
x= 11 y = 29
Customization options
=====================

