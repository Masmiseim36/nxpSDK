Overview
========
The pflash example shows how to use flash driver to operate program flash:



Toolchain supported
===================
- IAR embedded Workbench 8.11.3
- Keil MDK 5.23
- GCC ARM Embedded 6-2017-q2
- Kinetis Development Studio IDE 3.2.0
- MCUXpresso10.1.0

Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-KV31F board
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

