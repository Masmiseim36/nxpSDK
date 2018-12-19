Overview
========
The example demonstrates graphical widgets of the emWin library.

Running the demo
================
Known issue: The MCU is not able to generate hardware reset of the LCD due to hardware limitation of the boards interconnection.
Because of this the LCD may not get initialized (stays blank) following a cold start (power on) of the board even if the demo was
correctly programmed to the FLASH memory. In such a case simply press the reset button to restart the demo.
Hardware requirements
=====================
- Micro USB cable
- FRDM-K66F board
- Adafruit TFT LCD shield w/Cap Touch
- Single or double row pin connectors, 10 and 8 pins wide
- Personal Computer

Board settings
==============
Attach the LCD shield to the FRDM board.
You may need to use pin connectors as raisers to fit the LCD shield because of the height of the RJ45 connector on the FRDM-K66F board.

Prepare the Demo
================
1.  Connect a micro USB cable between the PC host and the OpenSDA USB port on the board.
2.  Build the project.
3.  Download the program to the target board.
4.  Reset the SoC and run the project.


Toolchain supported
===================
- IAR embedded Workbench  8.32.1
- Keil MDK  5.26
- GCC ARM Embedded  7.3.1
- MCUXpresso 10.3.0

