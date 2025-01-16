Hardware requirements
=====================
- Micro USB cable
- MIMXRT700-EVK board
- Personal Computer

Board settings
============

Prepare the Demo
===============
1.  Connect a micro USB cable between the PC host and the CMSIS DAP USB port (J1) on the board
2.  Open a serial terminal with the following settings (See Appendix A in Getting started guide for description how to determine serial port number):
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
The log below shows the output of the ELS (Crypto Sub System) and PKC (Public-Key Crypto Coprocessor) demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ELS PKC common example

============================
ELS get info:pass
RNG PRNG random:pass
Flow protection:pass
Memory compare:pass
Memory copy:pass
Key component operations:pass
ELS power down wake-up init:pass
PKC power down wake-up init:pass

============================
RESULT: All 8 test PASS!!
ELS example END
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
