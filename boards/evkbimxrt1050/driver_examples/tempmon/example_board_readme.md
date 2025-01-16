Hardware requirements
=====================
- Mini/micro USB cable
- EVKB-IMXRT1050 board
- Personal Computer

Board settings
==============

Prepare the Demo
================
1. Connect a USB cable between the PC host and the OpenSDA USB port on the board.
2. Open a serial terminal with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
When the example runs successfully, the following message is displayed in the terminal:
~~~~~~~~~~~~~~~~~~~~~~~~
TEMPMON driver example.
The chip initial temperature is 40.8 ℃.
The chip temperature has reached high temperature that is 42.7 ℃.
The chip throttling back core frequency to waiting a desired cool down temperature .
The chip core frequency is 62500000 Hz.
The chip temperature has reached low temperature that is 39.5 ℃.
The chip will return to the normal process .
The chip core frequency is 500000000 Hz.
~~~~~~~~~~~~~~~~~~~~~~~

