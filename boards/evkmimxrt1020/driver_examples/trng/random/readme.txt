Overview
========
The True Random Number Generator (TRNG) is a hardware accelerator module that generates a 512-bit
entropy as needed by an entropy consuming module or by other post processing functions. The TRNG
Example project is a demonstration program that uses the KSDK software to generate random numbers
and prints them to the terminal.



Toolchain supported
===================
- Keil MDK 5.24a
- IAR embedded Workbench 8.22.2
- GCC ARM Embedded 7-2017-q4-major
- MCUXpresso10.2.0

Hardware requirements
=====================
- Mini/micro USB cable
- EVK-MIMXRT1020 board
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
When the example runs successfully, the following message is displayed in the terminal:

RNGA Peripheral Driver Example
Generate 10 random numbers:
Random[0] = 0xE1554295
Random[1] = 0x827AD456
Random[2] = 0x9A1CBE1E
Random[3] = 0x4354CB53
Random[4] = 0xFE3B2494
Random[5] = 0xEDAB3F7D
Random[6] = 0x9AB91722
Random[7] = 0x4F54D999
Random[8] = 0x492414D1
Random[9] = 0x84611992

 Press any key to continue...

Note:
To debug in qspiflash, following steps are needed:
1. Select the flash target and compile.
3. Set the SW8: 1 off 2 off 3 on 4 off, then power on the board and connect USB cable to J23.
4. Start debugging in IDE.
   - Keil: Click "Download (F8)" to program the image to qspiflash first then clicking "Start/Stop Debug Session (Ctrl+F5)" to start debugging.
Customization options
=====================

