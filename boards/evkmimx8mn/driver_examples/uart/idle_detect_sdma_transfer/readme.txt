Overview
========
The uart_idle_detect_sdma example shows how to use uart driver in sdma way:

In this example, one uart instance connect to PC through uart, the board will
send back all characters that PC send to the board.

Uart will receive 8 characters every time, but if the character is less then 8, the idle
line interrupt will generate, and abort the SDMA receive operation, and send out the received
characters.

Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- GCC ARM Embedded  8.3.1

Hardware requirements
=====================
- Micro USB cable
- MIMX8MN6-EVK  board
- J-Link Debug Probe
- 12V power supply
- Personal Computer

Board settings
==============
No special settings are required.



Prepare the Demo
================
1.  Connect 12V power supply and J-Link Debug Probe to the board, switch SW101 to power on the board
2.  Connect a USB cable between the host PC and the J901 USB port on the target board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the demo runs successfully, the log would be seen on the debug terminal like:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Uart sdma transfer example!
Uart will receive 8 charactes every time, if less characters were received, 
Uart will generate the idle line detect interrupt, SDMA receive operation will be aborted.
Board will send the received characters out.
Now please input:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Customization options
=====================

