Overview
========
After power on, the board will start flashing all LEDs and will print the application logo into a serial port terminal, signalling an idle state.
To start the application the [ENTER] key on the keyboard. Then follow the onscreen instructions to configure and run available tests.

This demo requires a serial port terminal application through a serial interface.

For more information please refer to the "Kinetis MKW41Z Generic FSK Link Layer Software Quick Start Guide.pdf" document.

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
5.  Press [ENTER] in the console to display the main menu, or any other key to display the logo.
6.  Follow onscreen instructions to configure and run any available test.
7.  Repeat the steps above to prepare a second board for tests that require more than one.