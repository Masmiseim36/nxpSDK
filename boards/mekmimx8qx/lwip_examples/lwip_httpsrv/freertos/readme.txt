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
2.  On the browser address bar, type 192.168.0.102(IP address of the board).
	The browser should show a web page. The board also advertises itself using mDNS so that it can be accessed using URL http://lwip-http.local.
	Please note that your system may not support mDNS out-of-the-box as it is necessary to have an mDNS resolver installed.
    For instance Bonjour Print Services for Windows contain such resolver. In case of Linux nss-mdns serves this purpose.

Modifying content of static web pages
To modify content available through the web server you must complete following steps:
  1. Modify, add or delete files in folder "boards\<board_name>\lwip_examples\lwip_httpsrv_freertos\webpage".
  2. Run the script file "middleware\lwip\src\apps\httpsrv\mkfs\mkfs.pl <directory name>" to generate new "httpsrv_fs_data.c".
     Make sure to execute it from a folder where the file "httpsrv_fs_data.c" is. For example:
        C:\sdk\boards\<board_name>\lwip_examples\lwip_httpsrv_freertos> C:\sdk\middleware\lwip\src\apps\httpsrv\mkfs\mkfs.pl webpage
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
  3. Make sure the "httpsrv_fs_data.c" file has been overwritten with the newly generated content.
  4. Re-compile the HTTP server application example and download it to your board.
