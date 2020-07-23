Overview
========
This application implements a Bluetooh Low Energy HCI Black Box which can communicate over a serial interface.
To use the application Test Tool 12 or any other application which supports the HCI protocol is required.
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
3.  Press the reset button SW2 on your board to begin running the demo.
4.  Open Test Tool 12 and use the following settings with the detected serial device:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control

Running the demo
================
Use Test Tool 12 or any other application which supports the HCI protocol to interact with the device.
