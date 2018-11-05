Overview
========
The pflash example shows how to use flash driver to operate program flash:



Toolchain supported
===================
- Keil MDK 5.24a
- IAR embedded Workbench 8.22.2
- GCC ARM Embedded 7-2017-q4-major
- MCUXpresso10.2.0

Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-KV11Z board
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
The log below shows the output of the hello world demo in the terminal window:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 PFlash Example Start

 PFlash Information:

 Total Program Flash Size:      128 KB, Hex: (0x20000)

 Program Flash Sector Size:     2 KB, Hex: (0x800)

 Flash is UNSECURE!

 Erase a sector of flash

 Successfully Erased Sector 0x1f800 -> 0x20000

 Program a buffer to a sector of flash

 Successfully Programmed and Verified Location 0x1f800 -> 0x1f810

 End of PFlash Example
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

