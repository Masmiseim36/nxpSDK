Overview
========
The lwip_httpscli_ota demo application demonstrates an HTTPS OTA client using FreeRTOS + LWIP + MbedTLS + CoreHTTP.
The client connects to a HTTPS server and requests an OTA binary for download and subsequent installation by MCUBoot.


Toolchain supported
===================
- IAR embedded Workbench  9.20.2
- Keil MDK  5.36
- GCC ARM Embedded  10.3.1
- MCUXpresso  11.5.0

Hardware requirements
=====================
- Micro USB cable
- MIMXRT1170-EVK board
- Personal Computer


Board settings
==============
Demo is setup to use M.2 WiFi module.

Platform specific behavior
--------------------------
When an application programmatically performs software-induced reset while in a debugging session, the execution stalls in the ROM code. This is a feature.
Your attention is needed at that moment: please perform reset manually by pressing the on-board hw reset button once you spot "SystemReset" message in the serial console.
Manual reset is not needed while MCU is running freely without a debugger.

Prepare the Demo
================
1. The demo requires MCUBoot booloader to be present in the FLASH memory to function properly.
   It is recommended to build and program the bootloader first, then go on with the application.
   Please refer to respective readme of the mcuboot_opensource example and follow the steps there before you continue.
2. Go through setup files in the demo source tree source/config/ota_config.h and source/config/network_cfg.h to check:
    - use of DHCP/static IP
    - HTTPS server IP and port
    - OTA image filename as found on the server
3. Connect a USB cable between the PC host and the OpenSDA (or USB to Serial) USB port on the target board.
4. Open a serial terminal on PC for connected board with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
5.  Install WiFi connectivity module into SD or M.2 slot.


Running the HTTPS server example
--------------------------------

For better experience there is a simple HTTPS server Python3 implementation attached to this example.
It was tested with both Windows and Linux, however, it will run on anything supported by Python3.

The directory https_server has the following content:
    - ./certs - directory containing a private key and server certificate signed by mbedtls test_ca (found in mbedtls source)
    - ./httpsd_ota.py - https server code

When run with no parameters it creates a server instance on port 4433.
    - python3 httpsd_ota.py

With the server code running it is now possible to create connections to this server and request files located in its directory.
After the ota image is generated and signed, the last step is to copy it to the server directory.


Running the demo
================
To get the application properly executed by the bootloader, it is necessary to put signed application image to the primary application partition.
There are multiple options how to achieve that, however in principle the are two main methods (both presuming the bootlaoder is already in place):

a)  programing signed application image to the primary application partition using an external tool (direct method)
b)  jump-starting the application by debugger, performing OTA update with the signed image, resetting the board and letting the bootloader to perform update (indirect method)

The latter method is described step by step below:

1.  Open the demo project and build it.
    Known issue: MDK linker issues warning about unused boot_hdr sections. This does not affect the functionality of the example.
    
2.  Prepare signed image of the application from raw binary as described in the mcuboot_opensource readme.
     - In case of MCUXpress raw binary may not be generated automatically. Use binary tools after right clicking Binaries/.axf file in the project tree to generate it manually.

3.  Copy the signed image on your HTTPS server and make sure it's running. Default image path defined in the client is "/ota.bin".
    
4.  Launch the debugger in your IDE to jump-start the application.
     - In case of MCUXpresso IDE the execution stalls in an endless loop in the bootloader. Pause the debugging and use debugger console and issue command 'jump ResetISR'.
     
5.  When the demo runs successfully, the terminal will display message that coresponds with the following:

        Initializing WiFi connection...
        MAC Address: 48:E7:DA:9A:C7:B1
        [net] Initialized TCP/IP networking stack
        WLAN initialized
        WLAN FW Version: IW416-V0, RF878X, FP91, 16.91.21.p11.3, WPA2_CVE_FIX 1, PVE_FIX 1
        Successfully initialized WiFi module
        Joining: nxp-ota
        Connected to following BSS:SSID = [PlesCorp], IP = [192.168.0.114]
        Successfully joined: nxp-ota
        Got IP address 192.168.0.114
        
6. It will then try to establish secure connection to the server and starts downloading the OTA image:

        Getting size of requested file '/ota.bin'
        Determined file size is 271358 bytes
        Starting download of 271358 bytes with block size of 4096 bytes
        ...................................................................
        Download loop completed with size 271358, expected 271358
        
        MD5 hexdump of downloaded data:
        00000000  E2 BB 98 EF ED D0 C4 1D EB 70 EE 70 04 C8 68 0D |.........p.p..h.|
        
        MD5 hexdump of flashed data:
        00000000  E2 BB 98 EF ED D0 C4 1D EB 70 EE 70 04 C8 68 0D |.........p.p..h.|
        
        write magic number offset = 0x43ff00
        OTA image was downloaded successfully. Now press the reset button to reboot the board.

7. After image is downloaded successfully the user is prompted to restart the board.
   The bootloader will then install the new image - it will move it from the secondary partition to the primary one.

8. After installation of new image the application detects that it's now in testing phase. In this demo application this is
   simply done by marking the image as permanent and the user is requested to reboot the board again.

9. Application now starts and repeats the entire download process again, unless the new version was modified not to do so.
