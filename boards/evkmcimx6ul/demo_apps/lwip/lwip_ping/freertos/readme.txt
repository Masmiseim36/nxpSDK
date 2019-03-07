Overview
========

The lwip_ping demo application demonstrates a Ping Demo on the lwIP TCP/IP stack which using the ICMP protocol. The
application periodically sends the ICMP echo request to a PC and processes the PC reply. Type the "ping $board_address"
in the PC command window to send an ICMP echo request to the board. The lwIP stack sends the ICMP echo reply back to the
PC.


Toolchain supported
===================
- IAR embedded Workbench 7.80.4
- GCC ARM Embedded 2016-5.4-q3

Hardware requirements
=====================
- Micro USB cable
- MCIMX6UL-EVK  board
- Network cable RJ45 standard
- JLink Plus
- 5V power supply
- Personal Computer

Board settings
==============
No special is needed.
Note: 
1. This demo default use the Ethernet port 0, so please make sure the Network cable
is inserted into the first Ethernet port the one next to the usb port.
2. The RMII mode is used for default setting to initialize the ENET interface between MAC and the external PHY.

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
4.  Insert Cable to Ethernet RJ45 port 1 and connect it to your PC network adapter.
5.  Configure the host PC IP address to 192.168.0.100.
6.  Open a web browser.
7.  Download the program to the target board.
8.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.
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

