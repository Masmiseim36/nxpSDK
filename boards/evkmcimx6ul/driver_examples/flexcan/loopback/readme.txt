Overview
========
The flexcan_loopback_functional example shows how to use the loopback test mode to debug your CAN Bus design:

To demonstrates this example, only one board is needed. The example will config one FlexCAN Message
Buffer to Rx Message Buffer and the other FlexCAN Message Buffer to Tx Message Buffer with same ID.
After that, the example will send a CAN Message from the Tx Message Buffer to the Rx Message Buffer
throuth internal loopback interconnect and print out the Message payload to terminal.

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

==FlexCAN loopback functional example -- Start.==


Send message from MB9 to MB8

tx word0 = 0x11223344

tx word1 = 0x55667788


Receved message from MB8

rx word0 = 0x11223344

rx word1 = 0x55667788


==FlexCAN loopback functional example -- Finish.==
~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

