Overview
========

The lwip_ping demo application demonstrates a Ping Demo on the lwIP TCP/IP stack, which uses the ICMP protocol. The
application periodically sends the ICMP echo request to a PC, and processes the PC reply. Type "ping $board_address"
in the PC command window to send an ICMP echo request to the board. The lwIP stack sends the ICMP echo reply back to the
PC.


Toolchain supported
===================
- IAR embedded Workbench  8.32.1
- GCC ARM Embedded  7.3.1

Hardware requirements
=====================
- Micro USB cable
- i.MX8QM MEK Board
- J-Link Debug Probe
- 12V power supply
- Network cable RJ45 standard
- Personal Computer

Board settings
==============
No special setting needed.

#### Please note this application can't support running with Linux BSP! ####
This example aims to show the basic usage of the IP's function, some of the used Pads/Resources are also used by Cortex-A core.

Prepare the Demo
================
1.  Connect 12V power supply and J-Link Debug Probe to the board.
2.  Connect a USB cable between the host PC and the Debug port on the board (Refer "Getting Started with MCUXpresso SDK for i.MX 8QuadMax.pdf" for debug port information).
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Insert Cable to Ethernet RJ45 port and connect it to your PC.
5.  Configure the host PC IP address to 192.168.0.100.
6.  Download the program to the target board (Please refer "Getting Started with MCUXpresso SDK for i.MX 8QuadMax.pdf" for how to run different targets).
7.  Launch the debugger in your IDE to begin running the example.

Running the demo
================
When the demo runs, the log would be seen on the terminal like:
	************************************************
	 PING example
	************************************************
	 IPv4 Address     : 192.168.0.102
	 IPv4 Subnet mask : 255.255.255.0
	 IPv4 Gateway     : 192.168.0.100
	************************************************
	ping: send
	192.168.0.100


	ping: recv
	192.168.0.100
	 3 ms

	ping: send
	192.168.0.100


	ping: recv
	192.168.0.100
	 3 ms

	ping: send
	192.168.0.100


	ping: recv
	192.168.0.100
	 3 ms
Customization options
=====================

