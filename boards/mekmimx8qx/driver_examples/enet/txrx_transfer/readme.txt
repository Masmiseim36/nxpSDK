Overview
========

The enet_rxtx example shows the simplest way to use ENET driver for simple 
frame receive and transmit.

1. This example shows how to initialize the ENET MAC.
2. How to use ENET MAC to receive and transmit frame.

The example transmits 20 number broadcast frame, print the frame length, source MAC address
and destination MAC address when receive frame.

Note, The RGMII mode is used for default setting to initialize the ENET interface between MAC and the external PHY. you
can change it to RMII mode as you wish. Please make sure the RMII Mode setting in the MAC is synchronize to the setting
for the external PHY.


Toolchain supported
===================
- IAR embedded Workbench  8.32.1
- GCC ARM Embedded  7.3.1

Hardware requirements
=====================
- Micro USB cable
- MIMX8QX MEK board
- J-Link Debug Probe
- 12V power supply
- Network cable RJ45 standard
- Personal Computer

Board settings
==============
J7 connected with PC through RJ45 network cable.

#### Please note this application can't support running with Linux BSP! ####
This example aims to show the basic usage of the IP's function, some of the used Pads/Resources are also used by Cortex-A core.

Prepare the Demo
================
1.  Connect 12V power supply and J-Link Debug Probe to the board.
2.  Connect a micro USB cable between the host PC and the J11 USB port on the cpu board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Insert Cable to Ethernet RJ45 port and connect it to your PC.
5.  Download the program to the target board.
6.  Launch the debugger in your IDE to begin running the example.

Running the demo
================
When the demo runs, the log would be seen on the debug terminal like:

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

