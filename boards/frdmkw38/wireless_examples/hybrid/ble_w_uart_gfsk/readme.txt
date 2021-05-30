Overview
========
The application implements a Hybrid Bluetooth - Generic FSK demo. 
The example works on the FRDM-KW38 board. For more information, please consult the BLE Demo Applications User's Guide.

Toolchain supported
===================
- IAR embedded Workbench (ide version details are in the Release Notes)
- MCUXpresso IDE (ide version details are in the Release Notes)
Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-KW38 board

Board settings
==============
No special board setting.

Prepare the Demo
================
1.  Connect a mini/micro USB cable between the PC host and the OpenSDA USB port on the board.
3.  Download the program to the target board.
4.  Press the reset button on your board to begin running the demo.

Low Power
========
When enabling low power, SW3 is used to get the board out of low power. Then you have 3s time to exercise the commands 
available in the BLE Demo Applications User's Guide, after which the board goes back to sleep.