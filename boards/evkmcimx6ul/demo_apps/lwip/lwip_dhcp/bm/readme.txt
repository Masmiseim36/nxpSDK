Overview
========

The lwip_dhcp demo application demonstrates a DHCP demo on the lwIP TCP/IP stack.
The application acts as a DHCP client and prints the status as it is progressing.
Once the interface is being bound to an IP address obtained from DHCP server, address information is printed.


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
4.  Insert Cable to Ethernet RJ45 port 1 and connect it to a router (or other DHCP server capable device).
5.  Download the program to the target board.
6.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.
Running the demo
================
When the demo runs, the log would be seen on the terminal like:
	
************************************************
 DHCP example
************************************************
 DHCP state       : SELECTING
 DHCP state       : REQUESTING
 DHCP state       : CHECKING
 DHCP state       : BOUND

 IPv4 Address     : 192.168.0.4
 IPv4 Subnet mask : 255.255.255.0
 IPv4 Gateway     : 192.168.0.1

Customization options
=====================

