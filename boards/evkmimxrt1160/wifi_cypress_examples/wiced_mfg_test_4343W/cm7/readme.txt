Overview
========
Manufacturing Test Application

The manufacturing test application is used to test the radio performance of the DUT (device under testing)
and to assist with regulatory certification. The application works in conjunction with a Cypress supplied
wireless LAN manufacturing test utility, known as 'wl', which can be found in this path:
    middleware\wiced\43xxx_Wi-Fi\libraries\test\wl_tool\

The test application reads packetized IOCTL commands from the PC via a serial UART and forwards the commands
to the Cypress Wi-Fi device on the WICED module. Responses to the IOCTLs from the Wi-Fi device are returned
over the UART to the PC.

More details about this Test Application can be found in WICED Studio from Cypress https://www.cypress.com/products/wiced-software



Toolchain supported
===================
- IAR embedded Workbench  8.50.9
- GCC ARM Embedded  9.3.1
- MCUXpresso  11.3.1

Hardware requirements
=====================
- 2x Micro USB cable
- MIMXRT1160-EVK board
- Personal Computer
- Murata 1DX or 1LV M.2 module
- Murata uSD M.2 Adapter
- 1 plug to receptable header cable
- USB to Com Converter

Board settings
==============
1.If you want to use the WIFI# LBEE5KL1DX-883 module integrated on the board(U16)
- Connect R366, R367, R369, R370, R371, R372, R404, R408, R787, R788, R789, R790, R791, R792, R823.
- Disconnect R838.
- Do not use M.2 interface(J54) and SD Card Socket(J15).
- Modify sdmmc_config.h, define macro BOARD_SDMMC_SD_CD_TYPE as kSD_DetectCardByHostDATA3.

2.If you want to use the M.2 interface(J54) to connect external wifi module
- Connect R366, R367, R369, R370, R371, R372, R404, R408, R823.
- Disconnect R787, R788, R789, R790, R791, R792, R838.
- Do not use  SD Card Socket(J15).
- Modify sdmmc_config.h, define macro BOARD_SDMMC_SD_CD_TYPE as kSD_DetectCardByHostDATA3.

3.If you want to use the SD Card Socket(J15) to connect external wifi module
- Connect J55.
- Disconnect R787, R788, R789, R790, R791, R792.
- Do not use M.2 interface(J54).
- Modify sdmmc_config.h, define macro BOARD_SDMMC_SD_CD_TYPE as kSD_DetectCardByGpioCD.
Please make sure the R136 is welded for card detection through GPIO.
Note: Since the GPIO CD pin is conflict with the ENET MDC pin on board, so once R136 is welded for sdcard detection, then ENET may not work properly.
It is recommended to provide power directly to Murata uSD M.2 Adapter board using secondary Micro USB cable.
Jumper J38 on the adapter board has to be set up accordingly to utilize external power.
The following pins between the MIMXRT1160-EVK board and Murata uSD M.2 Adapter are connected using the plug to receptable cables:
- MIMXRT1160-EVK board's connector J10, pin 10 to Murata uSD M.2 Adapter's connector J9, pin 3

Remove R1798.
Connect pin:
- RX of USB2COM to J9-4
- TX of USB2COM to J9-2
- GND of USB2COM to J10-14
Prepare the Demo
================
1.  Connect a USB cable between the PC host and the OpenSDA port on the target board.
2.  Connect the WiFi module to SD card slot.
3.  Download the program to the target board with the CMSIS-DAP or J-Link debugger.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.


Running the demo
================
1.  Open a command prompt and navigate to the middleware\wiced\43xxx_Wi-Fi\libraries\test\wl_tool\ direcotry.

2.  Enter the following command at the command prompt to verify that the application is working:
      wl<chip_name>.exe --serial 99 ver
      Where 99 corresponds to the COM port number of the DUT (COM port number of USB/UART converter, can be found in windows device manager)

Example of using the wl utility from command prompt:
    C:\mcu-sdk-2.0\middleware\wiced\43xxx_Wi-Fi\libraries\test\wl_tool>wl4343WA1.exe --serial 41 ver
    7.45 RC41.0
    wl0: Apr 30 2018 04:13:29 version 7.45.98.50 (r688715 CY WLTEST) FWID 01-68e4a7a6

    
To see log from debug console make these steps:
 - Connect the board with PC by USB/UART converter, details for connection are in section: Board settings.
 - Open a serial terminal on PC for serial device (USB/UART converter) with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control


The log below shows the output of the demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Initializing WiFi Connection... 
WLAN MAC Address : A0:C9:A0:3D:F9:2F
WLAN Firmware    : wl0: Apr 30 2018 04:13:29 version 7.45.98.50 (r688715 CY WLTEST) FWID 01-68e4a7a6
WLAN CLM         : API: 12.2 Data: 9.10.39 Compiler: 1.29.4 ClmImport: 1.36.3 Creation: 2018-04-11 22:31:04 
Successfully Initialized WiFi Connection 
Starting WL server
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

