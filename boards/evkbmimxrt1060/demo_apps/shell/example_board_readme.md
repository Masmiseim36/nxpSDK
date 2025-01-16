Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT1060-EVKB board
- Personal Computer

Board settings
============
No special settings are required.

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board. 
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Note
- This demo will use GPIO toggle which is only available on A1 silicon (MIMXRT106xxxxxA).

Running the demo
================
When the example runs successfully, the following message is displayed in the terminal:

~~~~~~~~~~~~~~~~~~~~~
SHELL (build: Jun 19 2020)
 * Copyright 2021 NXP

SHELL>> help

"help": Lists all the registered commands

"led arg1 arg2":
 Usage:
    arg1: 1|2|3|4...            Led index
    arg2: on|off                Led status

"exit": Exit program

SHELL>> led 1 on
SHELL>> led 1 off
SHELL>> led 2 on
LED index is wrongs
SHELL>>
~~~~~~~~~~~~~~~~~~~~~

Note:
    1,The shell information "SHELL (build: Jun 19 2020)" may be different, which depends on the compile date.
    2,If the command 'led x on'/'led x off' run success, the corresponding LEDx on board will turn on/turn off.
      But the valid "Led index" may be different, which depends how many LEDs are available on the specific board.
      For example: one board only have LED1/LED2, then shell demo will print "LED index is wrong" for all led index bigger than 2.

