Overview
========
This sample is a simple application that can be used to verify that the board can boot and render graphics to the screen.


Toolchain supported
===================
- IAR embedded Workbench  9.32.1
- MCUXpresso  11.7.0

Hardware requirements
=====================
- Micro USB cable
- EVK-MIMXRT595
- Personal Computer
- RM67162 smart MIPI panel

Board settings
==============
Connect RM67162 smart MIPI panel to J44.

Prepare the Demo
================
The demo uses RM67162 panel by default.

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
If this example runs correctly, the sample GUI is displayed.
