Overview
========
The EDMA memory to memory example is a simple demonstration program that uses the SDK software.
It excuates one shot transfer from source buffer to destination buffer using the SDK EDMA drivers.
The purpose of this example is to show how to use the EDMA and to provide a simple example for
debugging and further development.

Toolchain supported
===================
- IAR embedded Workbench  8.32.3
- GCC ARM Embedded  8.2.1

Hardware requirements
=====================
- Micro USB cable
- MIMX8MM6-EVK  board
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
When the example runs successfully, you can see the similar information from the terminal as below.
~~~~~~~~~~~~~~~~~~~~~
SDMA memory to memory transfer example begin.

Destination Buffer:
0       0       0       0

SDMA memory to memory transfer example finish.

Destination Buffer:
1       2       3       4
~~~~~~~~~~~~~~~~~~~~~

Customization options
=====================

