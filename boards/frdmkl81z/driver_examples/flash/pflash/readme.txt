Overview
========
The pflash example shows how to use flash driver to operate program flash:


Running the demo
================
When the example runs successfully, the following message is displayed in the terminal:

~~~~~~~~~~~~
 FLASH example Start
 Flash Information:
 Total Program Flash Size: xx KB, Hex: (xx)
 Program Flash Sector Size: xx KB, hex: (xx)
 ...
 Flash is xx
 Erase a sector of flash
 Successfully Erased Sector xx -> xx
 Program a buffer to a sector of flash
 Successfully Programmed and Verified Location xx -> xx
 End of FLASH example
~~~~~~~~~~~~
Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-KL81Z board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
1. Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2. Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the example.


Toolchain supported
===================
- IAR embedded Workbench  8.32.1
- Keil MDK  5.26
- GCC ARM Embedded  7.3.1
- MCUXpresso 10.3.0

