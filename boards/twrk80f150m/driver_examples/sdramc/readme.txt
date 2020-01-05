Overview
========

The sdramc example shows how to use SDRAM controller driver to initialize the external SDRAM chip.
After the SDRAM initialization, SDRAM address access is very simple.


Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- Keil MDK  5.29
- GCC ARM Embedded  8.3.1
- MCUXpresso  11.1.0

Hardware requirements
=====================
- Mini USB cable
- TWRK80F150M REVB board
- Personal Computer

Board settings
==============
no shunt on J6.
Note: Remove C11 on TWRK80F150M REVB board.

Prepare the Demo
================
1.  Connect a mini/micro USB cable between the PC host and the USB port on the board. 
2.  Open a serial terminal on PC for OpenSDA serial device with these settings:
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

 SDRAM Write Data and Read Data Succeed.

 SDRAM Example End.

Customization options
=====================

