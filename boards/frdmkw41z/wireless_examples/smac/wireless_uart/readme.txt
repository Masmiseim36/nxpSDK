Overview
========
After power on, the board will start flashing all LEDs, signalling an idle state.
To start the application press the user push button on the board. Write any data using a serial console application to send it over the air.

This demo requires a serial port terminal application through a serial interface. Default application configuration:
 - source address:      0xBEAD
 - pan address:         0xFACE
 - destination address: 0xFFFF
 - channel:             0x0B

For more information please refer to the "MKW41Z SMAC Demo Applications User's Guide.pdf" document.

Toolchain supported
===================
- IAR Embedded Workbench 7.80.4
- MCUXpresso IDE 10.0

Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-KW41Z board
- Personal Computer a serial port terminal application installed.

Board settings
==============
No special board setting.

Prepare the Demo
================
1.  Connect a mini/micro USB cable between the PC host and the OpenSDA USB port on the board.
2.  Open a serial terminal on PC for the detected serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Press the reset button.
5.  Press the user switch button to start the application. 
6.  Write any byte through the terminal application to send it over the air using default settings. 
7.  Any payload of a packet intended for the device will be displayed in the serial port terminal application.