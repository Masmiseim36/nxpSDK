Hardware requirements
=====================
- Micro USB cable
- MIMXRT700-EVK board
- Personal Computer

Board settings
============

Prepare the Demo
===============
1.  Connect a micro USB cable between the PC host and the CMSIS DAP USB port on the board
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
When the demo runs successfully, the terminal displays similar information like the following:
~~~~~~~~~~~~~~~~~~
PUFv3 driver example
1510000:TestSelftest:PASS
1510001:TestGenerateRandomData:PASS
1510002:TestEnroll:PASS
1510003:TestStartAfterEnroll:PASS
1510004:TestDeriveDeviceKey:PASS
1510005:TestRandomWrapUnwrap:PASS
1510006:TestUserKeyWrapUnwrap:PASS
1510007:TestStop:PASS
1510008:TestZeroize:PASS
-----------------------
9 Tests 0 Failures 0 Ignored