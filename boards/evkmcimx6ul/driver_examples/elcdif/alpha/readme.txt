Overview
========
The ELCDIF ALPHA project shows how to drive the use the eLCDIF alpha surface.
Int this example, there are two surfaces, the back ground surface is a blue
rectangle in the center of the frame, the overlay surface is full screen red.
If this example runs correctly, you can see two rectangles(a smaller blue rectangle
in the screen center, and a larger red rectangle), the alpha value of the two
rectangles change.

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

Board settings
==============
1. Connect the LCD8000-43T board to J901.

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
If this example runs correctly, two rectangles are shown in the screen.
Customization options
=====================

