Overview
========
The flexnvm_dflash example shows how to use flash driver to operate data flash:



Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- Keil MDK  5.29
- GCC ARM Embedded  8.3.1
- MCUXpresso  11.1.0

Hardware requirements
=====================
- Micro USB cable
- TWR-KE18F board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
1. Connect a micro USB cable between the PC host and the OpenSDA USB port on the board.
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
 FlexNVM DFlash Example Start 

 Flash is UNSECURE!

 DFlash Information: 
 Data Flash Base Address: (0x10000000) 
 Data Flash Total Size:	64 KB, Hex: (0x10000)
 Data Flash Sector Size:	2 KB, Hex: (0x800) 
 Erase a sector of Data Flash
 Successfully Erased Sector 0x1000f800 -> 0x10010000

 Program a buffer to a sector of Data Flash 
 Successfully Programmed and Verified Location 0x1000f800 -> 0x1000f810 

 End of FlexNVM Dflash Example
~~~~~~~~~~~~
Customization options
=====================

