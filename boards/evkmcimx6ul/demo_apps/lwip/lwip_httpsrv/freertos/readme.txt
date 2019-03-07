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
2.  On the browser address bar, type 192.168.0.102(IP address of the board).
	The browser should show a web page.

Modifying content of static web pages
To modify content available through the web server you must complete following steps:
  1. Modify, add or delete files in folder "middleware\lwip_2.0.0\src\apps\httpsrv\mkfs\web_pages".
  2. Run the script file "middleware\lwip_2.0.0\src\apps\httpsrv\mkfs\mkfs.pl <directory name>" to generate new "httpsrv_fs_data.c".
	 For example:
		C:\sdk\middleware\lwip_2.0.0\src\apps\httpsrv\mkfs> mkfs.pl webpage
		Processing file webpage/auth.html
		Processing file webpage/cgi.html
		Processing file webpage/favicon.ico
		Processing file webpage/help.html
		Processing file webpage/httpsrv.css
		Processing file webpage/index.html
		Processing file webpage/NXP_logo.png
		Processing file webpage/poll.html
		Processing file webpage/request.js
		Processing file webpage/ssi.shtml
		Processing file webpage/welcome.html
		Done.
  3. Overwrite the middleware\lwip_2.0.0\src\apps\httpsrv\httpsrv_fs_data.c file by the new generated middleware\lwip_2.0.0\src\apps\httpsrv\mkfs\httpsrv_fs_data.c file.
  4. Re-compile the HTTP server application example and download it to your board. 
Customization options
=====================

