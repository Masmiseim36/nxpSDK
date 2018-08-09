Overview
========
The example demonstrates graphical widgets of the emWin library.


Toolchain supported
===================
- Keil MDK 5.24a
- IAR embedded Workbench 8.22.2
- GCC ARM Embedded 7-2017-q4-major
- MCUXpresso10.2.0

Hardware requirements
=====================
- Micro USB cable
- FRDM-K22F board
- Adafruit TFT LCD shield w/Cap Touch
- Personal Computer

Board settings
==============
Attach the LCD shield to the FRDM board.

Prepare the Demo
================
1.  Connect a micro USB cable between the PC host and the OpenSDA USB port on the board.
2.  Download emWin library package from NXP site and unpack it to the middleware/emwin directory prior building the project.
3.  Download the program to the target board.
4.  Reset the SoC and run the project.

Running the demo
================
Known issue:
The MCU is not able to generate hardware reset of the LCD due to hardware limitation of the boards interconnection.
Because of this the LCD may not get initialized (stays blank) following a cold start (power on) of the board even if the demo was
correctly programmed to the FLASH memory. In such a case simply press the reset button to restart the demo.
Supported board revisions:
FRDM-K22F REV A is not supported, the example is validated on REV C and D.
Customization options
=====================

