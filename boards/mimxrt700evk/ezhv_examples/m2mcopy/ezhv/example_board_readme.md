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
Memory to memory copy demo - message from primary core side.
Memory to memory copy demo - message from ezhv core side.
Press any key to start the program.
DTCM_1 DST[0] = 0
DTCM_1 DST[1] = 1
DTCM_1 DST[2] = 2
DTCM_1 DST[3] = 3
DTCM_1 DST[4] = 4

DTCM_2 DST[0] = 0
DTCM_2 DST[1] = 1
DTCM_2 DST[2] = 2
DTCM_2 DST[3] = 3
DTCM_2 DST[4] = 4

SRAM DST[0] = 0
SRAM DST[1] = 1
SRAM DST[2] = 2
SRAM DST[3] = 3
SRAM DST[4] = 4

DTCM_1 DST[0] = 1
DTCM_1 DST[1] = 2
DTCM_1 DST[2] = 3
DTCM_1 DST[3] = 4
DTCM_1 DST[4] = 5
Memory to memory copy demo execute success.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

