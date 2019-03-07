Overview
========
The uart_auto_baudrate_detect example shows how to use uart auto baud rate detect feature:

In this example, one uart instance connect to PC through uart. First, we should send characters a or A to board. 
The boars will set baud rate automatic. After baud rate has set, the board will send back all characters that PC
send to the board.

Toolchain supported
===================
- IAR embedded Workbench 7.80.4
- GCC ARM Embedded 2016-5.4-q3

Hardware requirements
=====================
- Micro USB cable
- MCIMX6UL-EVK  board
- JLink Plus
- 5V power supply
- Personal Computer

Board settings
==============
No special is needed.

Prepare the Demo
================
1.  Connect 5V power supply and JLink Plus to the board, switch SW2001 to power on the board
2.  Connect a USB cable between the host PC and the J1901 USB port on the target board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
Set any baud rate in your terminal, and send character a or A to board, then  
When the demo runs successfully, the log would be seen on the debug terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
UART has detect one character A
Baud rate has been set automatic!
Board will send back received characters
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

