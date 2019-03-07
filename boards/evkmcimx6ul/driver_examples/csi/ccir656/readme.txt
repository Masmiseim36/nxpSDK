Overview
========
The CSI CCIR656 project shows how to receive the camera data using CSI driver,
the camera interface is CCIR656. In this example, you will see the camera input
image shown in the LCD. Please note that the camera input image resolution
might be smaller than the LCD panel, so part of the LCD panel might be blank.

Toolchain supported
===================
- IAR embedded Workbench 7.80.4
- GCC ARM Embedded 2016-5.4-q3

Hardware requirements
=====================
- Micro USB cable
- MCIMX6UL-EVK  board
- JLink Plus
- 5V power supply
- Personal Computer
- LCD8000-43T LCD board
- OV5640 camera module

Board settings
==============
1. Connect the LCD8000-43T board to J901.
2. Connect the OV5640 camera module to J1801.

Prepare the Demo
================
1.  Connect 5V power supply and JLink Plus to the board, switch SW2001 to power on the board.
2.  Connect a USB cable between the host PC and the J1901 USB port on the target board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the demo runs successfully, the camera received pictures are shown in the LCD.
Customization options
=====================

