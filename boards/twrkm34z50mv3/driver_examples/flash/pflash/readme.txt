Overview
========
The pflash example shows how to use flash driver to operate program flash:



Toolchain supported
===================
- IAR embedded Workbench  9.10.2
- Keil MDK  5.34
- GCC ARM Embedded  10.2.1
- MCUXpresso  11.4.0

Hardware requirements
=====================
- Mini USB cable
- TWR-KM34Z50MV3 board
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
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
When the demo runs successfully, the following message is displayed in the terminal:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 PFlash example Start
 PFlash Information:
 Total Program PFlash Size: xx KB, Hex: (xx)
 Program PFlash Sector Size: xx KB, hex: (xx)
 ...
 Flash is xx
 Erase a sector of flash
 Successfully Erased Sector xx -> xx
 Program a buffer to a sector of flash
 Successfully Programmed and Verified Location xx -> xx
 End of FLASH example
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
