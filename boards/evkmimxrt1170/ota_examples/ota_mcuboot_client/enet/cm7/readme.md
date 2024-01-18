Overview
========

This application demonstrates an HTTPS OTA client using FreeRTOS + LWIP + MbedTLS + CoreHTTP.
The client connects to a HTTPS server and requests an OTA binary for download and subsequent installation by MCUBoot.


SDK version
===========
- Version: 2.15.000

Toolchain supported
===================
- IAR embedded Workbench  9.40.1
- Keil MDK  5.38.1
- GCC ARM Embedded  12.2
- MCUXpresso  11.8.0

Hardware requirements
=====================
- Mini/micro USB cable
- Network cable RJ45 standard
- MIMXRT1170-EVK board
- Personal Computer

Board settings
==============
This example uses 1G port(J4) as default. If want to test 100M port(J3), please set the macro BOARD_NETWORK_USE_100M_ENET_PORT to 1.

Platform specific behavior
---------------------------
When an application programmatically performs software-induced reset while in a debugging session, the execution stalls in the ROM code. This is a feature.
Your attention is needed at that moment: please perform reset manually by pressing the on-board hw reset button once you spot "SystemReset" message in the serial console.
Manual reset is not needed while MCU is running freely without a debugger.

Prepare the Demo
================
1. The demo requires MCUBoot bootloader to be present in the FLASH memory to function properly.
   It is recommended to build and program the bootloader first, then go on with the application.
   Please refer to respective readme of the mcuboot_opensource example and follow the steps there before you continue.
2. Go through setup files in the demo source tree source/config/ota_config.h and source/config/network_cfg.h to check:
    - use of DHCP/static IP
    - Default HTTPS server IP and port
    - Default OTA image file path on the server
3. Connect a USB cable between the PC host and the OpenSDA (or USB to Serial) USB port on the target board.
4. Open a serial terminal on PC for connected board with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
5. Insert the Ethernet Cable into the target board's RJ45 port and connect it to your PC network adapter.

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

	OTA HTTPS client demo (Ethernet)

	Copyright  2022  NXP

	$

6. The demo is now waiting for user input. Use 'help' to see all available commands.

7. Check the HTTPS server is running.

8. With HTTPS server runnning the client now can download an OTA image using 'ota' command:

        $ ota 0 /ota.bin 192.168.1.10 4433
   
   This creates a secure connection to specified server 192.168.1.10 using port 4433 and request
   file stored at path /ota.bin. The downloaded data will be stored into secondary slot for image 0.
   Note that it depends on the used platform how many images it uses (see 'image info' command for details).

9. After succesfull download of an OTA image, the command 'image info' can be used to inspect the change
   in the secondary image slot:

	$ image info
	Image 0; name APP; state None:
	  Slot 0; slotAddr 8020000; slotSize 655360
	    <IMAGE APP_PRIMARY: size 373268; version 1.22.0.0>
	  Slot 1; slotAddr 8210000; slotSize 655360
	    <IMAGE APP_SECONDARY: size 384238; version 2.20.0.0>

10. Now the downloaded image needs to be set as "ReadyForTest" to inform the bootloader about a new image to be
    installed and tested upon next reboot:

	$ image test 0

    This change is reflected in the image state parameter set as "ReadyForTest" in the 'image info' command output.

11. To trigger installation of new image either reboot the board manually or use 'reboot' command.

12. After new image is installed and booted, again use the 'image' command to make sure that the image
    moved from secondary slot into primary slot and image state is now set as "Testing".

13. Since the image now happens to be in the Testing state, it needs to be accepted with 'image accept' command:
	
	$ image accept 0

    This makes sure that upon next reboot the image won't be reverted to its previous version.
