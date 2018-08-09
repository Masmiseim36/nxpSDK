Overview
========
This application is an 802.15.4 Black Box, which can be controlled by another MCU or by the PC, via commands sent 
through a serial interface.
To send commands from the PC the TestTool12 application can be used.
For more information please refer to the "IEEE 802.15.4 MAC Demo Applications User's Guide.pdf" document.

Toolchain supported
===================
- IAR Embedded Workbench 7.80.4
- MCUXpresso IDE 10.0

Hardware requirements
=====================
- USB-KW41Z board
- Personal Computer with TestTool 12 installed

Board settings
==============
No special board setting.

Prepare the Demo
================
1.  Connect the board to the PC host.
2.  Open "Command Console" from TestTool 12 and create connection to the device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Press the reset button on your board to begin running the demo.
    The device is now ready to receive command over the serial interface.

Running the demo
================
Send commands from TestTool 12 application