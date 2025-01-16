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
ELS hash example

============================
SHA224 one shot:pass
SHA256 one shot:pass
SHA256 streaming example:pass
SHA256 long message example:pass
SHA384 one shot:pass
SHA512 one shot:pass

============================
RESULT: All 6 test PASS!!
ELS example END
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
