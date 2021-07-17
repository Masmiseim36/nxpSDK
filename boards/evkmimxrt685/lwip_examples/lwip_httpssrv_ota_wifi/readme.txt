Overview
========

The lwip_httpsrv_ota_freertos demo application demonstrates OTA update through HTTPServer set up on lwIP TCP/IP and the MbedTLS stack with
FreeRTOS. The user uses an Internet browser to upload new version of firmware and restarts the board to perform the update


Toolchain supported
===================
- MCUXpresso  11.4.0
- IAR embedded Workbench  9.10.2

Hardware requirements
=====================
- Mini/micro USB cable
- EVK-MIMXRT685 board
- Personal Computer
- One of the following Wi-Fi modules:
  - Panasonic PAN9026 SDIO ADAPTER
  - AzureWave AW-NM191NF-uSD + uSD to SD adapter
  - AzureWave AW-AM457-uSD + uSD to SD adapter
  - AzureWave AW-CM358-uSD + uSD to SD adapter

Board settings
==============
This example, by default, is built to work with the AzureWave AW-AM457-uSD. It is configured by the macro definition in file app_config.h (#define WIFI_BOARD_AW_AM457).
If you want use the AzureWave AW-NM191NF-uSD, please change the macro to WIFI_BOARD_AW_NM191.
If you want use the Panasonic PAN9026 SDIO ADAPTER, please change the macro to WIFI_BOARD_PAN9026_SDIO.
If you want use the AzureWave AW-CM358-uSD, please change the macro to WIFI_BOARD_AW_CM358.

Jumper settings for AzureWave AW-NM191NF-uSD Module:
  - J11 1-2: VIO_SD 1.8V (Voltage level of SDIO pins is 1.8V)
  - J2  1-2: 3.3V VIO_uSD (Power Supply from uSD connector)

Jumper settings for AzureWave AW-AM457-uSD Module:
  - J11 1-2: VIO_SD 1.8V (Voltage level of SDIO pins is 1.8V)
  - J2  1-2: 3.3V VIO_uSD (Power Supply from uSD connector)

Jumper settings for AzureWave AW-CM358-uSD Module:
  - J4 1-2: VIO 1.8V (Voltage level of SDIO pins is 1.8V)
  - J2 1-2: 3.3V VIO_uSD (Power Supply from uSD connector)
  - The pin 1 of J4 is not marked on the board. Please note that pin numbering of J4 is opposite to J2 (pin 1 is close to the "J4" label):
         3 2 1
         o o=o J4
      J2 o=o o
         1 2 3

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
4.  Install WiFi connectivity module into SD or M.2 slot.

Running the demo
================
1.  Load the demo project and build it.
    Known issue: MDK linker issues warning about unused boot_hdr sections. This does not affect the functionality of the example.
2.  Launch the debugger in your IDE to begin running the demo.
3.  When the demo runs successfully, the terminal will display the following:

        Initializing WiFi connection...
        MAC Address: D8:C0:A6:C0:B0:57
        [net] Initialized TCP/IP networking stack
        WLAN initialized
        WLAN FW Version: w8978o-V0, RF878X, FP91, 16.91.10.p185, WPA2_CVE_FIX 1, PVE_FIX 1
        Successfully initialized WiFi module
        Starting Access Point: SSID: nxp-ota, Chnl: 1
        [wlcm] Warn: NOTE: uAP will automatically switch to the channel that station is on.
        Soft AP started successfully
        This also starts DHCP Server with IP 192.168.1.1
        Join 'nxp-ota' network and visit https://192.168.1.1

4. Connect you PC to WiFi network provided by the board (SSID: nxp-ota, password: NXP0123456789).
   Your PC should acquire IP configuration automatically via DHCP.
5. Open web browser and type https://192.168.1.1 (IP address of the board) on the browser address bar.
   The browser should show the main web page of the example.
   Note: Be sure to include "https" protocol specifier in the address bar, so that your browser attempts to establish secure connection to TCP port 443,
   as browsers generally tend to use non-secure connection to port 80 by default.
6. Go to OTA page, select file with udpated firmware and upload it.
7. After the file is uploaded, click "Reboot" button to start newly uploaded firmware in test mode.
8. Once the updated firmware executes, the "Accept update" button becomes active. Click it to make the update permanent.
   Note: Make sure your computer is connected to the AP provided by the board at this point as it might automatically connect to another AP during the reboot. 

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
