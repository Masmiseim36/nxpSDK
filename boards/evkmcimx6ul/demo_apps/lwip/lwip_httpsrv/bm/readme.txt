Overview
========

The lwip_httpsrv demo application demonstrates an HTTPServer on the lwIP TCP/IP stack with bare metal SDK or FreeRTOS.
The user uses an Internet browser to send a request for connection. The board acts as an HTTP server and sends a Web
page back to the PC.


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
1.  When the demo runs successfully, the terminal will display the following:
        ************************************************
         HTTP Server example
        ************************************************
         IPv4 Address     : 192.168.0.102
         IPv4 Subnet mask : 255.255.255.0
         IPv4 Gateway     : 192.168.0.100
        ************************************************
2.  On the browser address bar, type 192.168.0.102(ip address of the board).
    The browser should respond as shown below:

		lwIP - A Lightweight TCP/IP stack

		The web page you are watching was served by a simple web server running on top of the lightweight TCP/IP stack lwIP.

		lwIP is an open source implementation of the TCP/IP protocol suite that was originally written by Adam Dunkels of
		the Swedish Institute of Computer Science but now is being actively developed by a team of developers distributed
		world-wide.Since it's release, lwIP has spurred a lot of interest and has been ported to several platforms and
		operating systems. lwIP can be used either with or without an underlying OS.

		The focus of the lwIP TCP/IP implementation is to reduce the RAM usage while still having a full scale TCP. This
		makes lwIP suitable for use in embedded systems with tens of kilobytes of free RAM and room for around 40 kilobytes
		of code ROM.

		More information about lwIP can be found at the lwIP homepage at http://savannah.nongnu.org/projects/lwip/ or at
		the lwIP wiki at http://lwip.wikia.com/.

Customization options
=====================

