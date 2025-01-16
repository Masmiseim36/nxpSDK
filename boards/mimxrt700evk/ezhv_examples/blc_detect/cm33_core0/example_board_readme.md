Hardware requirements
=====================
- Micro USB cable
- MIMXRT700-EVK board
- Personal Computer

Board settings
============ 
Connect J11-5 to J11-6

Prepare the Demo
===============
1.  Connect a micro USB cable between the PC host and the USB port on the board
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
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
when normal event enabled (set DEMO_TEST_NORMAL_EVENT to true):

BLC detect demo - message from primary core side.
BLC detect demo - message from ezhv core side.
Normal event - press any key to let the ezhv core enter wait state.
Press any key to trigger output.
Output signal triggered.
EZHV core wake up by blc slice0 normal event.

when vector event enabled (set DEMO_TEST_VETORED_EVENT to true):

BLC detect demo - message from primary core side.
BLC detect demo - message from ezhv core side.
Vector event - Press any key to let the ezhv core enter wait state.
Press any key to trigger output.
Output signal triggered.
EZHV core wake up by blc slice0 vector event.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
