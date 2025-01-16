Hardware requirements
=====================
- Mini/micro USB cable
- EVK-MIMXRT1170 board
- Personal Computer

Board settings
============
Make sure nand flash is available on board.

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

Running the demo
===============
When the demo runs successfully, the log would be seen on the OpenSDA terminal like below, after shell is available, please start with command 
"help".
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
nand flash management demo.

Make file system..............

Create directory......

SHELL build: Oct  9 2020
Copyright  2020  NXP

SHELL>>

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
