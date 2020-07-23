Overview
========
The application implements a custom GATT based OTAP Profile.
To use the application Test Tool 12 or any other application which supports the FSCI protocol is required.
For more information, please consult the "BLE Demo Applications User's Guide".

Toolchain supported
===================
- IAR embedded Workbench (ide version details are in the Release Notes)
- MCUXpresso IDE (ide version details are in the Release Notes)

Hardware requirements
=====================
- usbkw38 board

Board settings
==============
No special board setting.

Known Issues
============
It is not possible to use embedded JLink software to debug target
Workaround: use external debugger and press SW3 while starting the debugger until halt

Prepare the Demo
================
1.  Plug USBKW38 to PC USB port
2.  Download the program to the target board.
3.  Press the reset button on your board to begin running the demo.
4.  Start the OTAP BLE application from Test Tool 12 and follow the instructions in the manual.

Running the demo
================
Interact with the device using Test Tool 12 or your custom application which supports the OTAP FSCi commamnds.
