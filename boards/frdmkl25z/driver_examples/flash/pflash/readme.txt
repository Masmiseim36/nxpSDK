Overview
========
The pflash example shows how to use flash driver to operate program flash:



Toolchain supported
===================
- IAR embedded Workbench 7.80.4
- Keil MDK 5.21a
- GCC ARM Embedded 2016-5.4-q3
- Kinetis Development Studio IDE 3.2.0
- MCUXpresso0.8

Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-KL25Z board
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
When the example runs successfully, you can see the similar information from the terminal as below.

 PFLASH example Start
 PFlash Information:
 Total Program Flash Size: xx KB, Hex: (xx)
 Program Flash Sector Size: xx KB, hex: (xx)
 Flash is UNSECURE!
 Erase a sector of flash
 Successfully Erased Sector xx -> xx
 Program a buffer to a sector of flash
 Successfully Programmed and Verified Location xx -> xx


 End of PFLASH example
Customization options
=====================

