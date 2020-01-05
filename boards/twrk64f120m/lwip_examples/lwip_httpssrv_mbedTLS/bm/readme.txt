Overview
========


The lwip_httpsrv_mbedTSL_bm demo application demonstrates an HTTPS Server
set up on lwIP TCP/IP and the mbedTLS stack with bare metal. The user
uses an Internet browser to send an https request for connection.
The board acts as an HTTPS server and sends a Web page back to the PC.


Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- Keil MDK  5.29
- GCC ARM Embedded  8.3.1
- MCUXpresso  11.1.0

Hardware requirements
=====================
- Mini/micro USB cable
- RJ45 Network cable
- TWR-K64F120M board
- TWR-SER board
- Personal Computer

Board settings
==============
TWR-SER:
    - RMII mode - J2 3-4 shunt on, J3 2-3 shunt on, J12 shunt on 9,10.
Prepare the Demo
================
1.  Connect a mini USB cable between the PC host and the OpenSDA(or USB to Serial) USB port on the board.
2.  Open a serial terminal on PC for OpenSDA serial(or USB to Serial) device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Insert Cable to Ethernet RJ45 port and connect it to your PC.
4.  Configure ip address of PC to 192.168.0.100 .
5.  Open a web browser.
6.  Download the program to the target board.
7.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
1.  When the demo runs successfully, the terminal will display the following:
		Initializing PHY...

		************************************************
		 mbedTLS HTTPS Server example
		************************************************
		 IPv4 Address     : 192.168.0.102
		 IPv4 Subnet mask : 255.255.255.0
		 IPv4 Gateway     : 192.168.0.100
		************************************************
2.  On the browser address bar, type https://192.168.0.102 (ip address of the board).
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

