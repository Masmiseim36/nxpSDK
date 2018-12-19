Overview
========

The lwip_httpsrv_wolfssl demo application demonstrates an HTTPServer set up on lwIP TCP/IP and the WolfSSL stack with
FreeRTOS. The user uses an Internet browser to send an https request for connection. The board acts as an HTTP server
and sends a Web page back to the PC.

Prepare the Demo
================
1.  Connect a USB cable between the PC host and the OpenSDA(or USB to Serial) USB port on the target board.
2.  Open a serial terminal on PC for OpenSDA serial(or USB to Serial) device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Insert the Ethernet Cable into the target board's RJ45 port and connect it to your PC network adapter.
4.  Configure the host PC IP address to 192.168.0.100.
5.  Open a web browser.
6.  Download the program to the target board.
7.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
1.  When the demo runs successfully, the terminal will display the following:
        ************************************************
         WolfSSL HTTPS Server example
        ************************************************
         IPv4 Address     : 192.168.0.102
         IPv4 Subnet mask : 255.255.255.0
         IPv4 Gateway     : 192.168.0.100
        ************************************************
2.  On the browser address bar, type https://192.168.0.102(IP address of the board).
	The browser should show a web page.

Modifying content of static web pages
To modify content available through the web server you must complete following steps:
  1. Modify, add or delete files in folder "middleware\lwip_2.0.3\src\apps\httpsrv\mkfs\web_pages".
  2. Run the script file "middleware\lwip_2.0.3\src\apps\httpsrv\mkfs\mkfs.pl <directory name>" to generate new "httpsrv_fs_data.c".
	 For example:
		C:\sdk\middleware\lwip_2.0.3\src\apps\httpsrv\mkfs> mkfs.pl webpage
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
  3. Overwrite the middleware\lwip_2.0.3\src\apps\httpsrv\httpsrv_fs_data.c file by the new generated middleware\lwip_2.0.3\src\apps\httpsrv\mkfs\httpsrv_fs_data.c file.
  4. Re-compile the HTTP server application example and download it to your board. 
Hardware requirements
=====================
- Mini/micro USB cable
- RJ45 Network cable
- FRDM-K66F board
- Personal Computer

Board settings
==============
No special settings are required.


Toolchain supported
===================
- IAR embedded Workbench  8.32.1
- Keil MDK  5.26
- GCC ARM Embedded  7.3.1
- MCUXpresso 10.3.0

