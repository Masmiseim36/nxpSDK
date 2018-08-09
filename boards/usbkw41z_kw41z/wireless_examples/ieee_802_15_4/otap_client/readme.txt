Overview
========
This application represents the "802.15.4 Client" of the Over-the-Air Programming Demo, and it demonstrates the following:
- scanning for a coordinator
- association procedure to a Personal Area Network (PAN)
- the downloading procedure of a new image from the Server
- the image upgrade procedure via the Bootloader application.
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
3.  Download the Bootloader program to the target board.
4.  Download the application program to the target board.
5.  Press the reset button, then any other switch button on your board to begin running the demo.

Running the demo
================
Follow the instruction displayed on the console.