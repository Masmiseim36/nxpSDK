Overview
========
The PXP queue project shows how to use the PXP command queue. This example
use the command queue to rotate the process surface. If this example
runs correctly, you will see a square with three color(red, green and blue).
The rotate degree is changing.

Toolchain supported
===================
- Keil MDK 5.25
- IAR embedded Workbench 8.30.2
- GCC ARM Embedded 7-2017-q4-major
- MCUXpresso10.2.1

Hardware requirements
=====================
- Mini/micro USB cable
- EVK-MIMXRT1060 board
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
When the demo runs successfully, the log would be seen on the OpenSDA terminal like:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
PXP command queue example start...
Customization options
=====================

