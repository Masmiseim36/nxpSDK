Overview
========
The serial_terminal demo shows how to use emWin library to render text. Connect using serial terminal application to debug console and type text.
Please note that this example does not make use of receive buffer thus this example cannot handle larger amout of text pasted to the terminal at a time.

Toolchain supported
===================
- IAR embedded Workbench  8.50.9
- Keil MDK  5.33
- GCC ARM Embedded  9.3.1
- MCUXpresso  11.3.0

Hardware requirements
=====================
- Micro USB cable
- FRDM-K28FA board
- TFT Proto 5" CAPACITIVE board by Mikroelektronika (www.mikroe.com) HW REV 1.01
- Personal Computer

Board settings
==============
Attach the LCD shield to the FRDM board.

Prepare the Demo
================
1.  Connect a micro USB cable between the PC host and the OpenSDA USB port on the board.
2.  Build the project.
3.  Download the program to the target board.
4.  Reset the SoC and run the project.
