Overview
========
This application implements a Bluetooth Low Energy Host Black Box which can communicate using the FSCI protocol over a serial interface.
To use the application Test Tool 12 or any other application which supports the FSCI protocol is required.
For more information, please consult the "BLE Demo Applications User's Guide".

Toolchain supported
===================
- IAR embedded Workbench (ide version details are in the Release Notes)
- MCUXpresso IDE (ide version details are in the Release Notes)

Hardware requirements
=====================
- Mini/micro USB cable
- frdmkw38 board

Board settings
==============
No special board setting.
To flash the board in case the sensor is put in deep sleep, press RESET.

Prepare the Demo
================
1.  Connect a mini/micro USB cable between the PC host and the OpenSDA USB port on the board.
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
Use Test Tool 12 or any other application which supports the FSCI protocol to interact with the device.
