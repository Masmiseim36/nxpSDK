Overview
========

The lwip_ping demo application demonstrates a Ping Demo on the lwIP TCP/IP stack, which uses the ICMP protocol. The
application periodically sends the ICMP echo request to a PC, and processes the PC reply. Type "ping $board_address"
in the PC command window to send an ICMP echo request to the board. The lwIP stack sends the ICMP echo reply back to the
PC.


Toolchain supported
===================
- IAR embedded Workbench  8.50.9
- GCC ARM Embedded  9.3.1

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
No special setting needed.

#### Please note this application can't support running with Linux BSP! ####
This example aims to show the basic usage of the IP's function, some of the used Pads/Resources are also used by Cortex-A core.

Prepare the Demo
================
1.  Connect 12V power supply and J-Link Debug Probe to the board, switch SW3 to power on the board.
2.  Connect a micro USB cable between the host PC and the J11 USB port on the cpu board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Insert the Ethernet Cable into the target board's RJ45 port and connect it to your PC network adapter.
5.  Download the program to the target board.
6.  Configure the host PC IP address to 192.168.0.100.
7.  Launch the debugger in your IDE to begin running the example.

Running the demo
================
When the demo runs, the log would be seen on the terminal like:
	Initializing PHY...

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
	 4 ms

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

