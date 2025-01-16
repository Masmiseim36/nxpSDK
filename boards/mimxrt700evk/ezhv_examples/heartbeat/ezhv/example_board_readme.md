Hardware requirements
=====================
- Micro USB cable
- MIMXRT700-EVK board
- Personal Computer

Board settings
============ 

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
when vector event enabled (set DEMO_TEST_VETORED_EVENT to true):

HeartBeat demo - message from primary core side.
HeartBeat demo - message from ezhv core side.
EZHV heartbeat vector event example.
Press any key to start the program.
EZHV core wake up by heartbeat vector event.
EZHV core wake up by heartbeat vector event.
EZHV core wake up by heartbeat vector event.


when normal event enabled (set DEMO_TEST_NORMAL_EVENT to true):

HeartBeat demo - message from primary core side.
HeartBeat demo - message from ezhv core side.
EZHV heartbeat normal event example.
Press any key to start the program.
EZHV wake up by heartbeat normal event.
EZHV wake up by heartbeat normal event.
EZHV wake up by heartbeat normal event.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
