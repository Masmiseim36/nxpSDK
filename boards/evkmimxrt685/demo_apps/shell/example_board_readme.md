Hardware requirements
=====================
- Micro USB cable
- EVK-MIMXRT685 board
- Personal Computer

Board settings
============

Prepare the Demo
===============
1.  Connect a micro USB cable between the PC host and the CMSIS DAP USB port (J5) on the board
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Launch the debugger in your IDE to begin running the demo.

Running the demo
================
The log below shows the output of the shell demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SHELL (build: Aug 21 2018)
Copyright  2017  NXP

SHELL>> help

"help": Lists all the registered commands

"led arg1 arg2":
 Usage:
    arg1: 1|2|3|4...            Led index
    arg2: on|off                Led status

"exit": Exit program

SHELL>> led 1 on
SHELL>> led 2 off
LED index is wrong
SHELL>>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
