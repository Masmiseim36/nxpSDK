Hardware requirements
=====================
- Micro USB cable
- EVK-MIMXRT685 board
- Personal Computer

Board settings
============

Prepare the Demo
===============
1.  Connect a micro USB cable between the PC host and the CMSIS DAP USB port (J5) on the board
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
The log below shows the output of the iap otp example in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
IAP OTP example
Please input the OTP word index to read(0~511):4
Fuse[4] value: 0x403B

Please input the OTP word index to read(0~511):
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
