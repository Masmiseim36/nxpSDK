Overview
========
The bubble application demonstrates basic usage of the on-board accelerometer to implement a
bubble level. A bubble level utilizes two axes to visually show deviation from a level plane
(0 degrees) on a given access.


Toolchain supported
===================
- Keil MDK 5.24a
- IAR embedded Workbench 8.22.2
- GCC ARM Embedded 7-2017-q4-major
- MCUXpresso10.2.0

Hardware requirements
=====================
- Mini/Micro USB cable
- FRDM-KL46Z board
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
4.  Unplug the usb cable about 10 second then plug again to begin running the demo.

Running the demo
================

indicates X-axis angle while another indicates Y-axis angle.

  When the demo runs successfully, the following message is displayed in the terminal:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Welcome to the BUBBLE example

You will see the change of LED brightness on board and angle data in the console when change the angles of board

x=  6 y = 22
x=  8 y = 26
x= 10 y = 28
x= 10 y = 28
x= 11 y = 29
x= 11 y = 29
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

