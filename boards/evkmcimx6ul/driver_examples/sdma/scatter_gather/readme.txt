Overview
========
The SDMA scatter gather example is a simple demonstration program that uses the SDK software.
It excuates several shots transfer from source buffer to destination buffer using the SDK SDMA drivers.
The purpose of this example is to show how to use the SDMA and to provide a scatter gather example for
debugging and further development.

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
When the example runs successfully, following information can be seen on the terminal:

~~~~~~~~~~~~~~~~~~~~~
sdma scatter gather transfer example begin.

Destination Buffer:
0       0       0       0       0       0       0       0       0       0       0       0       0       0       0       0

sdma scatter gather transfer example finish.

Destination Buffer:
0       1       2       3       4       5       6       7       8       9       10      11      12      13      14      15
~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

