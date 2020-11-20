Overview
========
The example demonstrates graphical widgets of the emWin library.


Toolchain supported
===================
- IAR embedded Workbench  8.50.5
- Keil MDK  5.31
- GCC ARM Embedded  9.2.1
- MCUXpresso  11.2.0

Hardware requirements
=====================
- Micro USB cable
- LPCXpresso55S69 board
- Adafruit TFT LCD shield w/Cap Touch V2.3
- Personal Computer

Board settings
==============
Attach the LCD shield to the LPC board.

Prepare the Demo
================
Note: MCUXpresso IDE project default debug console is semihost
1.  Power the board using a micro USB cable connected to P5 USB port on the board, attach debugger to P7 connector
2.  Build the project.
3.  Download the program to the target board.
4.  Reset the SoC and run the project.

Running the demo
================
Known issue: The MCU is not able to generate hardware reset of the LCD due to hardware limitation of the boards interconnection.
Because of this the LCD may not get initialized (stays blank) following a cold start (power on) of the board even if the demo was
correctly programmed to the FLASH memory. In such a case simply press the reset button to restart the demo.
If user use Adafruit TFT LCD V2.8, please refer to this link: https://community.nxp.com/community/mcuxpresso/blog/2020/01/10/modifying-the-latest-adafruit-28-lcd-for-sdk-graphics-examples
