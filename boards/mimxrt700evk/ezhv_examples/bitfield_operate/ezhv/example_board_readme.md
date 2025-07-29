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
Bitfield operate demo - message from primary core side.
Bitfield operate demo - message from ezhv core side.
Press any key to start the project.

========== Bit field extract instructions test ==========
The extracted data after execute the bfextiu instruction is 0x43.
The extracted data after execute the bfextu instruction is 0x43.

========== Bit field insert instructions test ==========
The destination data after execute the bfinsi instruction is 0x87654321.
The destination data after execute the bfins instruction is 0x87654321.

========== Bit field set instructions test ==========
The destination data after execute the bfseti instruction is 0x876ff321.
The destination data after execute the bfset instruction is 0x876ff321.

========== Bit field clear instructions test ==========
The destination data after execute the bfclri instruction is 0x87600321.
The destination data after execute the bfclr instruction is 0x87600321.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
