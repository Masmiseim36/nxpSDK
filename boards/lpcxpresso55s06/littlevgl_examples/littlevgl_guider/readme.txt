Overview
========

This project is used to work with LittleVGL GUI Guider. The GUI Guider generated
code is placed in the folder "generated". The example generated code shows a
button in the panel.

Toolchain supported
===================
- IAR embedded Workbench  9.10.2
- GCC ARM Embedded  10.2.1
- MCUXpresso  11.4.0
- Keil MDK  5.34

Hardware requirements
=====================
- Micro USB cable
- LPCXpresso55S06 board
- Adafruit TFT LCD shield w/Cap Touch V2.3
- Personal Computer

Board settings
==============
There are different versions of Adafruit 2.8" TFT LCD shields. The shields marked
v2.0 works directly with this project. For the other shields, please solder
the center pin of IOREF pads to the 3.3V pad, and solder the pads 11, 12, and 13.
See the link for details:
https://community.nxp.com/t5/MCUXpresso-Community-Articles/Modifying-the-latest-Adafruit-2-8-quot-LCD-for-SDK-graphics/ba-p/1131104

Attach the LCD shield to the LPC board.

Short JP25 1-2, JP26 1-2, JP9, Remove JP12
This project does not use the serial port J1, J3-1 for UART RX, J3-2 for UART TX.

Prepare the Demo
================
1.  Power the board using a micro USB cable connected to J2 USB port on the board, attach debugger to J1 connector
2.  Build the project.
3.  Download the program to the target board.
4.  Reset the SoC and run the project.

Running the demo
================
Known issue: The MCU is not able to generate hardware reset of the LCD due to hardware limitation of the boards interconnection.
Because of this the LCD may not get initialized (stays blank) following a cold start (power on) of the board even if the demo was
correctly programmed to the FLASH memory. In such a case simply press the reset button to restart the demo.
If user use Adafruit TFT LCD V2.8, please refer to this link: https://community.nxp.com/community/mcuxpresso/blog/2020/01/10/modifying-the-latest-adafruit-28-lcd-for-sdk-graphics-examples
