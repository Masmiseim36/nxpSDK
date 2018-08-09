Overview
========
This application represents the "802.15.4 Coordinator" of the My Wireless Application Host Demo, and it demonstrates the following:
- communicating with a black box application through a serial interface
- FSCI Host functionality
- channel selection algorithm
- the creation of a basic Personal Area Network (PAN)
- device association procedure
- the exchange of data between the coordinator and the device.
This demo requires a console application to monitor the messages.
For more information please refer to the "IEEE 802.15.4 MAC Demo Applications User's Guide.pdf" document.

Toolchain supported
===================
- IAR Embedded Workbench 7.80.4
- MCUXpresso IDE 10.0

Hardware requirements
=====================
- USB-KW41Z board
- Personal Computer

Board settings
==============
No special board setting.

Prepare the Demo
================
1.  Connect the board to the PC host.
2.  Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the "802.15.4 Black Box" application into the KW41Z of the target board.
4.  Download the program to the K22F of the target board.
5.  Press both reset buttons, then any other switch button on your board to begin running the demo.

Running the demo
================
Follow the instruction displayed on the console.