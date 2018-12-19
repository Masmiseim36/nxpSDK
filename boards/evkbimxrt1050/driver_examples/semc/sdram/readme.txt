Overview
========

The sdramc example shows how to use SEMC controller driver to initialize the external SDRAM chip.


Hardware requirements
=====================
- Mini/micro USB cable
- EVKB-IMXRT1050 board
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
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the demo runs successfully, the log would be seen on the OpenSDA terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEMC SDRAM Example Start!

 SEMC SDRAM Memory 32 bit Write Start, Start Address 0x80000000, Data Length 4096 !

 SEMC SDRAM Read 32 bit Data Start, Start Address 0x80000000, Data Length 4096 !

 SEMC SDRAM 32 bit Data Write and Read Compare Start!

 SEMC SDRAM 32 bit Data Write and Read Compare Succeed!

 SEMC SDRAM Memory 16 bit Write Start, Start Address 0x80000000, Data Length 4096 !

 SEMC SDRAM Read 16 bit Data Start, Start Address 0x80000000, Data Length 4096 !

 SEMC SDRAM 16 bit Data Write and Read Compare Start!

 SEMC SDRAM 16 bit Data Write and Read Compare Succeed!

 SEMC SDRAM Memory 8 bit Write Start, Start Address 0x80000000, Data Length 4096 !

 SEMC SDRAM Read 8 bit Data Start, Start Address 0x80000000, Data Length 4096 !

 SEMC SDRAM 8 bit Data Write and Read Compare Start!

 SEMC SDRAM 8 bit Data Write and Read Compare Succeed!

 SEMC SDRAM Example End.


~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Toolchain supported
===================
- IAR embedded Workbench  8.32.1
- Keil MDK  5.26
- GCC ARM Embedded  7.3.1
- MCUXpresso 10.3.0

