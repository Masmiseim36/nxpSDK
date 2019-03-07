Overview
========

The enet_rxtx example shows the simplest way to use ENET driver for simple 
frame receive and transmit.

1. This example shows how to initialize the ENET MAC.
2. How to use ENET MAC to receive and transmit frame.

The example transmits 20 number broadcast frame, print the frame length, source MAC address
and destination MAC address when receive frame.

Note, The RMII mode is used for default setting to initialize the ENET interface between MAC and the external PHY. you
can change it to MII mode as you wish. Please make sure the MII Mode setting in the MAC is synchronize to the setting
in TWR-SERIAL board for the external PHY.

Toolchain supported
===================
- IAR embedded Workbench 7.80.4
- GCC ARM Embedded 2016-5.4-q3

Hardware requirements
=====================
- Network cable RJ45 standard
- Micro USB cable
- MCIMX6UL-EVK  board
- JLink Plus
- 5V power supply
- Personal Computer

Board settings
==============

Prepare the Demo
================
1.  Connect 5V power supply and JLink Plus to the board, switch SW2001 to power on the board.
2.  Connect a USB cable between the host PC and the J1901 USB port on the target board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Insert Cable to Ethernet RJ45 port 1 and connect it to your PC.
5.  Download the program to the target board.
6.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the demo runs, the log would be seen on the terminal like:

ENET example start.

The 1 frame transmitted success.
The 2 frame transmitted success.
The 3 frame transmitted success.
The 4 frame transmitted success.
......
The 20 frame transmitted success.

the transmitted frame is a 1000 length broadcast frame. the frame can be seen
when the PC is installed with wireshark.

when a 1000 length frame is received, the log would be added to the terminal like:
A frame received. the length 1000 Dest Address xx:xx:xx:xx:xx:xx Src Address xx:xx:xx:xx:xx:xx
Customization options
=====================

