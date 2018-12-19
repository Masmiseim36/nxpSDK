Overview
========
The pflash example shows how to use flash driver to operate program flash:


Running the demo
================
The log below shows the output of the hello world demo in the terminal window:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 PFlash Example Start

 PFlash Information:

 Total Program Flash Size:      1024 KB, Hex: (0x100000)

 Program Flash Sector Size:     8 KB, Hex: (0x2000)

 Flash is UNSECURE!

 Erase a sector of flash

 Successfully Erased Sector 0x100fe000 -> 0x10100000

 Program a buffer to a sector of flash

 Successfully Programmed and Verified Location 0x100fe000 -> 0x100fe010

 End of PFlash Example
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Hardware requirements
=====================
- Mini/micro USB cable
- Jlink Segger Plus
- TWR-KV58F220M board
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


Toolchain supported
===================
- IAR embedded Workbench  8.32.1
- Keil MDK  5.26
- GCC ARM Embedded  7.3.1
- MCUXpresso 10.3.0

