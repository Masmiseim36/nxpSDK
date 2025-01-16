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
Arm to ezhv interrupt request demo - message from primary core side.
Arm to ezhv interrupt request demo - message from ezhv core side.
Press any key to let the ezhv core enter wait state.
Send MEI request from arm to ezhv.
Ezhv received arm core mei request.
Ezhv exit wait state.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
