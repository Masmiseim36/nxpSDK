Overview
========

The lwip_udpecho demo application demonstrates a UDP echo demo on the lwIP TCP/IP stack with bare metal KSDK or
FreeRTOS, which uses the UDP protocol and acts as an echo server. The application sends back the received UDP packets
from the PC, which can be used to test whether a UDP connection is available.


Toolchain supported
===================
- IAR embedded Workbench 7.80.4
- GCC ARM Embedded 2016-5.4-q3

Hardware requirements
=====================
- Mini/micro USB cable
- Network cable RJ45 standard
- MCIMX6UL-EVK  board
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
1. When the demo runs, the log would be seen on the terminal like:
		************************************************
		 UDP Echo example
		************************************************
		 IPv4 Address     : 192.168.0.102
		 IPv4 Subnet mask : 255.255.255.0
		 IPv4 Gateway     : 192.168.0.100
		************************************************
2. From the command window, type echotool 192.168.0.102 /p udp /r 7 /d hello
	NOTE: The echotool.exe application can be found here https://github.com/PavelBansky/EchoTool
3. The command window should get response:
		Hostname 192.168.0.102 resolved as 192.168.0.102

		Reply from 192.168.0.102:7, time 1 ms OK
		Reply from 192.168.0.102:7, time 1 ms OK
		Reply from 192.168.0.102:7, time 0 ms OK
		Reply from 192.168.0.102:7, time 0 ms OK
		Reply from 192.168.0.102:7, time 0 ms OK

		Statistics: Received=5, Corrupted=0
Customization options
=====================

