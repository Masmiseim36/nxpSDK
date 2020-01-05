Overview
========
The pflash example shows how to use flash driver to operate program flash:



Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- Keil MDK  5.29
- GCC ARM Embedded  8.3.1
- MCUXpresso  11.1.0

Hardware requirements
=====================
- Micro USB cable
- FRDM-KE15Z board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2. Open a serial terminal with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
When the example runs successfully, the following message is displayed in the terminal

~~~~~~~~~~~~
 PFlash Example Start 

 PFlash Information: 
 Total Program Flash Size:	256 KB, Hex: (0x40000)
 Program Flash Sector Size:	2 KB, Hex: (0x800) 
 Flash is UNSECURE!

 Erase a sector of flash
 Successfully Erased Sector 0x3f800 -> 0x40000

 Program a buffer to a sector of flash 
 Successfully Programmed and Verified Location 0x3f800 -> 0x3f810 

 End of PFlash Example
~~~~~~~~~~~~
Customization options
=====================

