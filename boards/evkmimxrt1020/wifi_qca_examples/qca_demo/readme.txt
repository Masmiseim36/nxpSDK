Overview
========
The qca_demo example provides basic commands to show the functionality of qca_wifi stack.
The default IP configuration is "addr: 192.168.1.10 mask: 255.255.255.0 gw: 192.168.1.1".
To obtain a valid IP configuration from DHCP server, press 'd' after WiFi connection is established.

Toolchain supported
===================
- IAR embedded Workbench  8.32.3
- Keil MDK  5.27
- GCC ARM Embedded  8.2.1
- MCUXpresso  11.0.0

Hardware requirements
=====================

Board settings
==============
Plug GT202 Adaptor board to Arduino stackable headers.
Remove the resistor R137.

Known issues:
- HTTP request to gateway (by pressing 'w') may retrun corrupted data.

Please use external power supply (J2), USB may not be sufficient.


Note:
To debug in qspiflash, following steps are needed:
1. Select the flash target and compile.
3. Set the SW8: 1 off 2 off 3 on 4 off, then power on the board and connect USB cable to J23.
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

2.  Connect a micro USB cable between the PC host and the SDA port
3.  Open a serial terminal with the following settings (See Appendix A in Getting started guide for description how to determine serial port number):
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

  s  AP Scan
  c  AP Connect (SSID='nxp', pass='NXP0123456789')
  D  AP Disconnect
  d  Get DHCP address
  g  HTTP GET nxp.com
  w  HTTP GET from gateway
  p  Ping gateway
  P  Ping nxp.com
  i  Print IP configuration
  R  Resolve some hosts
  h  Help (print this menu)
  H  Print extended help

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- Press 's' to scan available Wifi networks. In the list you should see the "nxp" network.
- Press 'c' to connect to "nxp" network.
- Press 'd' to get an IP address by DHCP.
- Press 'g' to download several bytes of NXP website.

Customization options
=====================

