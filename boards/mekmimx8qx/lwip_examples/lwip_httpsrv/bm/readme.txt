Overview
========

The lwip_httpsrv demo application demonstrates an HTTPServer on the lwIP TCP/IP stack with bare metal SDK or FreeRTOS.
The user uses an Internet browser to send a request for connection. The board acts as an HTTP server and sends a Web
page back to the PC.


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
1.  When the demo runs successfully, the terminal will display the following:
        Initializing PHY...

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

