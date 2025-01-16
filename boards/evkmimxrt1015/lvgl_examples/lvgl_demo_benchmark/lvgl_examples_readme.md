Hardware requirements
=====================
- Micro USB cable
- EVK-MIMXRT1015 board
- Personal Computer
- Adafruit TFT LCD shield w/Cap Touch

Board settings
============
There are different versions of Adafruit 2.8" TFT LCD shields. The shields marked
v2.0 works directly with this project. For the other shields, please solder
the center pin of IOREF pads to the 3.3V pad, and solder the pads 11, 12, and 13.
See the link for details:
https://community.nxp.com/t5/MCUXpresso-Community-Articles/Modifying-the-latest-Adafruit-2-8-quot-LCD-for-SDK-graphics/ba-p/1131104

1. Attach the LCD shield to the board.
2. Connect jumper J21
(Note: If we do not connect jumper J21, LCD may not display correctly when debugging 
because it can't get reset signal.)

Prepare the Demo
================
1.  Connect a micro USB cable between the PC host and the OpenSDA USB port on the board.
2.  Build the project.
3.  Download the program to the target board.
4.  Reset the SoC and run the project.

Running the demo
===============
If this example runs correctly, the sample GUI is displayed.
