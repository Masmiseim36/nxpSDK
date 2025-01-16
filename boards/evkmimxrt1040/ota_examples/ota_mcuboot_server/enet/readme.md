# ota_mcuboot_server_enet

## Overview

This application demonstrates OTA update through HTTP server based on lwIP TCP/IP stack with
FreeRTOS. The user uses an Internet browser to upload new version of firmware and restarts the board to perform the update

## Supported Boards
- [EVKB-IMXRT1050](../../../_boards/evkbimxrt1050/ota_examples/ota_mcuboot_server/enet/example_board_readme.md)
- [MIMXRT1060-EVKB](../../../_boards/evkbmimxrt1060/ota_examples/ota_mcuboot_server/enet/example_board_readme.md)
- [MIMXRT1170-EVKB](../../../_boards/evkbmimxrt1170/ota_examples/ota_mcuboot_server/enet/example_board_readme.md)
- [MIMXRT1060-EVKC](../../../_boards/evkcmimxrt1060/ota_examples/ota_mcuboot_server/enet/example_board_readme.md)
- [MIMXRT1040-EVK](../../../_boards/evkmimxrt1040/ota_examples/ota_mcuboot_server/enet/example_board_readme.md)
- [EVK-MIMXRT1064](../../../_boards/evkmimxrt1064/ota_examples/ota_mcuboot_server/enet/example_board_readme.md)
- [MIMXRT1160-EVK](../../../_boards/evkmimxrt1160/ota_examples/ota_mcuboot_server/enet/example_board_readme.md)
- [MIMXRT1180-EVK](../../../_boards/evkmimxrt1180/ota_examples/ota_mcuboot_server/enet/example_board_readme.md)
- [FRDM-MCXN947](../../../_boards/frdmmcxn947/ota_examples/ota_mcuboot_server/enet/example_board_readme.md)
- [FRDM-RW612](../../../_boards/frdmrw612/ota_examples/ota_mcuboot_server/enet/example_board_readme.md)
- [MCX-N5XX-EVK](../../../_boards/mcxn5xxevk/ota_examples/ota_mcuboot_server/enet/example_board_readme.md)
- [MCX-N9XX-EVK](../../../_boards/mcxn9xxevk/ota_examples/ota_mcuboot_server/enet/example_board_readme.md)
- [RD-RW612-BGA](../../../_boards/rdrw612bga/ota_examples/ota_mcuboot_server/enet/example_board_readme.md)

## Prepare the Demo
1. The demo requires MCUBoot bootloader to be present in the FLASH memory to function properly.
   It is recommended to build and program the bootloader first, then go on with the application.
   Please refer to respective readme of the mcuboot_opensource example and follow the steps there before you continue.
2. Prior launching the demo it is recommended to pre-build image with modified version of the application to test the OTA update process.
3. Connect a USB cable between the PC host and the OpenSDA(or USB to Serial) USB port on the target board.
4. Open a serial terminal on PC for OpenSDA serial(or USB to Serial) device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
    - Unix line ending using `\n`
5. Insert the Ethernet Cable into the target board's RJ45 port and connect it to your PC network adapter.
6. Configure the host PC IP address to 192.168.0.100.

## Running the demo
To get the application properly executed by the bootloader, it is necessary to put signed application image to the primary application partition.
There are multiple options how to achieve that, however in principle the are two main methods (both presuming the bootlaoder is already in place):

1.  programing signed application image to the primary application partition using an external tool (direct method)
2.  jump-starting the application by debugger, performing OTA update with the signed image, resetting the board and letting the bootloader to perform update (indirect method)

**The latter method is described step by step below:**

1.  Load the demo project and build it.
    Known issue: MDK linker issues warning about unused boot_hdr sections. This does not affect the functionality of the example.
    
2.  Prepare signed image of the application from raw binary as described in the mcuboot_opensource readme.
     - In case of MCUXpress raw binary may not be generated automatically. Use binary tools after right clicking Binaries/.axf file in the project tree to generate it manually.
    
3.  Launch the debugger in your IDE to jump-start the application.
     - In case of MCUXpresso IDE the execution stalls in an endless loop in the bootloader. Pause the debugging and use debugger console and issue command `jump ResetISR`.
     
4.  When the demo runs successfully, the terminal will display the following:

        Initializing PHY...

        ************************************************
         mbedSSL HTTP Server example
        ************************************************
         IPv4 Address     : 192.168.0.102
         IPv4 Subnet mask : 255.255.255.0
         IPv4 Gateway     : 192.168.0.100
        ************************************************
5. Open web browser and type `http://192.168.0.102` (IP address of the board) on the browser address bar.
   The browser should show the main web page of the example.
   Note: Be sure to include *http* protocol specifier in the address bar, so that your browser attempts to establish HTTP connection using port 80
6. Go to OTA page, select file with udpated firmware (signed image) and upload it.
7. After the file is uploaded, click *Reboot* button to start newly uploaded firmware in test mode.
8. Once the updated firmware executes, the *accept* button becomes active. Click it to make the update permanent.

Known issue:
Browser may not display progress of the upload. This is an issue of the browser rather than OTA demo.
It is possible to watch upload progress in the attached serial console.

## Modifying content of static web pages
To modify content available through the web server you must complete following steps:
  1. Modify, add or delete files in folder `boards\<board_name>\lwip_examples\lwip_httpsrv_ota\webpage`.
  2. Run the script file `middleware\lwip\src\apps\httpsrv\mkfs\mkfs.pl <directory name>` to generate new `httpsrv_fs_data.c`.
     Make sure to execute it from a folder where the file `httpsrv_fs_data.c` is. For example:

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
		 
  3. Make sure the `httpsrv_fs_data.c` file has been overwritten with the newly generated content.
  4. Re-compile the HTTP server application example and download it to your board. 

