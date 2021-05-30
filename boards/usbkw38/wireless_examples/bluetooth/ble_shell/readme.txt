Overview
========
The application implements a Bluetooth Low Energy Host with a shell inteface.
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
The details for switching between USB-KW38 dongle as a Bluetooth Low Energy sniffer
and USB-KW38 as a development platform can be found at the bellow link:
https://www.nxp.com/document/guide/getting-started-with-the-usb-kw38:GS-USB-KW38

Known Issues
============
It is not possible to use embedded JLink software to debug target
Workaround: use external debugger and press SW3 while starting the debugger until halt

Prepare the Demo
================
1.  Plug USB-KW38 to PC USB port
2.  Download the program to the target board.
3.  Press the reset button on your board to begin running the demo.
4.  Open a serial terminal application and use the following settings with the detected serial device:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control

Running the demo
================
Use a serial terminal to interact with the device.
