Overview
========
After power on, the board will start flashing all LEDs and will print the application logo into a serial port terminal, signalling an idle state.
To start the application the [ENTER] key on the keyboard. Then follow the onscreen instructions to configure messenger functionality and to start sending messages.

This demo requires a serial port terminal application through a serial interface.

For more information please refer to the "MKW41Z SMAC Demo Applications User's Guide.pdf" document.

Toolchain supported
===================
- IAR Embedded Workbench 7.80.4
- MCUXpresso IDE 10.0

Hardware requirements
=====================
- USB-KW41Z board
- Personal Computer a serial port terminal application installed.

Board settings
==============
No special board setting.

Prepare the Demo
================
1.  Connect the USB-KW41Z platform to an USB port.
2.  Open a serial terminal on PC for the detected serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Press the reset button.
5.  Press [ENTER] in the console to display the main menu, or any other key to display the logo.
6.  Use the keys displayed in the shortcuts menu to configure channel, transmission power and number of retransmissions in case the channel is busy or acknowledge message is not received.
7.  Use the first menu entry to configure addressing information. Input values using ASCII HEX characters and press [ENTER] to validate them.
8.  Use the third menu entry to input AES128-CBC key and initial vector. Input values are ASCII HEX.
9.  Use the second menu entry to start console mode. Type any text and press [ENTER] to send it. Press [ESC] to exit console mode.
10. Repeat the steps above to prepare a second board to communicate with the first one, provided that correct addressing and channel are configured.