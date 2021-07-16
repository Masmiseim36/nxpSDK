Overview
========

The lwip_httpsrv_ota_freertos demo application demonstrates OTA update through HTTPServer set up on lwIP TCP/IP and the MbedTLS stack with
FreeRTOS. The user uses an Internet browser to upload new version of firmware and restarts the board to perform the update


Toolchain supported
===================
- IAR embedded Workbench  9.10.2
- Keil MDK  5.34
- GCC ARM Embedded  10.2.1
- MCUXpresso  11.4.0

Hardware requirements
=====================
- Mini/micro USB cable
- Ethernet cable
- EVK-MIMXRT1060 board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
1. Prior launching the demo it is recommended to pre-build modified version of the application in .bin format to test the OTA update process.
   It is also required to have ota_bootloder programmed to the FLASH memory. Please see readme for the ota_bootloader for details.
2. Connect a USB cable between the PC host and the OpenSDA(or USB to Serial) USB port on the target board.
3. Open a serial terminal on PC for OpenSDA serial(or USB to Serial) device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Insert the Ethernet Cable into the target board's RJ45 port and connect it to your PC network adapter.

Running the demo
================
1.  Load the demo project and build it.
    Known issue: MDK linker issues warning about unused boot_hdr sections. This does not affect the functionality of the example.
2.  Launch the debugger in your IDE to begin running the demo.
3.  When the demo runs successfully, the terminal will display the following:
        Initializing PHY...

        ************************************************
         mbedSSL HTTPS Server example
        ************************************************
         IPv4 Address     : 192.168.0.102
         IPv4 Subnet mask : 255.255.255.0
         IPv4 Gateway     : 192.168.0.100
        ************************************************
4. Configure the host PC IP address to 192.168.0.100.
5. Open web browser and type https://192.168.0.102 (IP address of the board) on the browser address bar.
   The browser should show the main web page of the example.
   Note: Be sure to include "https" protocol specifier in the address bar, so that your browser attempts to establish secure connection to TCP port 443,
   as browsers generally tend to use non-secure connection to port 80 by default.
6. Go to OTA page, select file with udpated firmware and upload it.
7. After the file is uploaded, click "Reboot" button to start newly uploaded firmware in test mode.
8. Once the updated firmware executes, the "Accept update" button becomes active. Click it to make the update permanent.

Known issue:
Browser may not display progress of the upload. This is an issue of the browser rather than OTA demo.
It is possible to watch upload progress in the attached serial console.

Modifying content of static web pages
To modify content available through the web server you must complete following steps:
  1. Modify, add or delete files in folder "boards\<board_name>\lwip_examples\lwip_httpsrv_ota\webpage".
  2. Run the script file "middleware\lwip\src\apps\httpsrv\mkfs\mkfs.pl <directory name>" to generate new "httpsrv_fs_data.c".
     Make sure to execute it from a folder where the file "httpsrv_fs_data.c" is. For example:
        C:\sdk\boards\<board_name>\lwip_examples\lwip_httpssrv_ota> C:\sdk\middleware\lwip\src\apps\httpsrv\mkfs\mkfs.pl webpage
		Processing file webpage/favicon.ico
        Processing file webpage/httpsrv.css
        Processing file webpage/index.html
        Processing file webpage/NXP_logo.png
        Processing file webpage/ota.shtml
        Processing file webpage/ota_reboot.html
        Processing file webpage/request.js
        Processing file webpage/welcome.html
		Done.
  3. Make sure the "httpsrv_fs_data.c" file has been overwritten with the newly generated content.
  4. Re-compile the HTTP server application example and download it to your board. 
