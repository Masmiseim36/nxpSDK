Overview
========
This sample is a simple application that can be used to verify that the board can boot and render graphics to the screen.


Toolchain supported
===================
- IAR embedded Workbench  9.10.2
- MCUXpresso  11.4.0

Hardware requirements
=====================
- Micro USB cable
- EVKB-IMXRT1050 board
- Personal Computer
- RK043FN02H-CT LCD board

Board settings
==============
1. Connect the RK043FN02H-CT board to J8.

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
Following text is displayed in the console:
***********************************************
* Crank Software Storyboard Demo
* Storyboard Runtime version :6.2.0 build 0
* Target platform : IMXRT1050-EVKB
* Target OS : FreeRTOS V10.2.1
* Target Filesystem : SBVFS
***********************************************
