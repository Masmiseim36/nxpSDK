Overview
========
The example demonstrates how do use the LittlevGL terminal. In this example,
the left half of the screen is used as terminal, when input from the debug
terminal, the input characters are shown in the terminal in the screen. The
input characters could be cleared by clicking the button "Clear".  When click
the cross in the title, the terminal is closed, the input characters is not
shown anymore.

Toolchain supported
===================
- IAR embedded Workbench  8.32.3
- Keil MDK  5.27
- GCC ARM Embedded  8.2.1
- MCUXpresso  11.0.1

Hardware requirements
=====================
- Micro USB cable
- EVK-MIMXRT1010 board
- Personal Computer
- Adafruit TFT LCD shield w/Cap Touch

Board settings
==============
- Attach the LCD shield to the board.
- Weld 0Ω resistor to R795.
- Connect jumper J21
(Note: If we do not connect jumper J21, LCD may not display correctly when debugging 
because it can't get reset signal.)

Prepare the Demo
================
1.  Connect a micro USB cable between the PC host and the OpenSDA USB port on the board.
2.  Build the project.
3.  Download the program to the target board.
4.  Reset the SoC and run the project.

Running the demo
================
If this example runs correctly, the sample GUI is displayed.
Customization options
=====================

