Overview
========
The qca_throughput demo tests transfer throughput.



Toolchain supported
===================
- IAR embedded Workbench  8.32.3
- Keil MDK  5.26
- GCC ARM Embedded  7.3.1
- MCUXpresso  10.3.1

﻿Hardware requirements
======================
- Mini/micro USB cable
- EVK-MIMXRT1015 board
- SX-ULPAN-2401-SHIELD(US)
- Personal Computer

Board settings
==============
Plug SX-ULPAN-2401-SHIELD(US) board to FRDM stackable headers (J17, J18, J19, J20).
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SX-ULPAN-2401-SHIELD(US) board  connect  evkmimxrt1015 board
                CN11-4         ---        J30-1
                CN11-6         ---        J30-3
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Known issues:
- HTTP request to gateway (by pressing 'w') may retrun corrupted data.


Note:
To debug in qspiflash, following steps are needed:
1. Select the flash target and compile.
3. Set the SW8: 1 off 2 off 3 on 4 off, then power on the board and connect USB cable to J41.
4. Start debugging in IDE.
   - Keil: Click "Download (F8)" to program the image to qspiflash first then clicking "Start/Stop Debug Session (Ctrl+F5)" to start debugging.
Prepare the Demo
================

1.  Configure your dedicated device to create a WiFi network with
    SSID "nxp" and password "NXP0123456789"

    If you want to use notebook as an AP, please follow these instructions:
    a.  Download https://github.com/oblique/create_ap.git
    b.  Run "make install"
    c.  Connect ethernet cable with internet access to notebook.
    d.  Run command "sudo create_ap wlan0 eth0 nxp NXP0123456789"
    where 
        wlan0 - represent Linux Wifi device
        eth0  - represent Linux ethernet device
        nxp   - is a WiFi SSID
        NXP0123456789 - is a WiFi password

    The names of ethernet and WiFi device might be different.
    Find names in your system by running of command "ifconfig"

2.  Download and install FBENCH on your PC. FBENCH is available at:
    https://sourceforge.net/projects/fbench/

3.  Connect a micro USB cable between the PC host and the SDA port
4.  Open a serial terminal with the following settings (See Appendix A in Getting started guide for description how to determine serial port number):
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Reset the SoC and run the project.


Running the demo
================
When the demo runs successfully, the log would be seen on the CMSIS DAP terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Initializing WIFI driver...
WLAN driver initialized in 251ms with 0 irqs
WLAN driver malloc'ed 7374 bytes (not counting OS structures!)
Host version :  3.3.0.0
Target version   :  0x31c80997
Firmware version :  3.3.4.91
Interface version:  1
wifiIrqs=0

SHELL (build: Jan 10 2017)
Copyright (c) 2015 Freescale Semiconductor
SHELL>>

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

How to run throughput test:

1) Connect your PC to "nxp" WiFi network. Let's assume that PC IP address in "nxp' network is 192.168.12.35 and GT202 will get IP 192.168.12.99.

2) Download demo to board and run following commands to connect GT202 to "nxp" WiFi.

    wmiconfig --p NXP0123456789
    wmiconfig --wpa 2 CCMP CCMP
    wmiconfig --connect nxp
    # obtain an IP by DHCP
    wmiconfig --ipdhcp

3) Run 'benchrx tcp 7007' command to receive TCP data on port 7007.
4) Launch FBENCH, switch to "IPV4 Receiving" tab.
   Set "local IP address" to 192.168.12.35 (or 0.0.0.0), "Local Port" to 7008 and press "Listen/Receive"
5) Run 'benchtx 192.168.12.35 7008 tcp 1024 0 600 0'
   Now the GT202 is transfering data to PC for 10 minutes, using packet size 1024B.
6) Launch new instance of FBENCH, switch to tab "IPV4 Sending".
   Set "Remote IP address" to IP 192.168.12.99 and "Remote Port" to 7007 and click to "Connect/Send".
   In this moment, the PC is transfering data to GT202.
7) Final test results are available in each FBENCH instance.

For more options, please type "help" or "wmiconfig --help".

Full description of former throughput demo is available at "GT202 Demo Applications User Guide 1.3.pdf". The document can be found on ARROW website: https://static4.arrow.com/-/media/images/part-detail-pages/gt202-kits/gt202-wifi-module-api-guide-v1-3.pdf

GT202 communication is described in "GT202 MQX PDK Demo Applications 1.3.pdf".
https://static4.arrow.com/-/media/images/part-detail-pages/gt202-kits/gt202-mqx-pdk-demo-applications-user-guide-1-3.pdf

Customization options
=====================

