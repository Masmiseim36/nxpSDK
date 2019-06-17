Overview
========
Manufacturing Test Application

The manufacturing test application is used to test the radio performance of the DUT (device under testing)
and to assist with regulatory certification. The application works in conjunction with a Cypress supplied
wireless LAN manufacturing test utility, known as 'wl', which can be found in this path:
    middleware\wifi_wiced\libraries\test\wl_tool\

The test application reads packetized IOCTL commands from the PC via a serial UART and forwards the commands
to the Cypress Wi-Fi device on the WICED module. Responses to the IOCTLs from the Wi-Fi device are returned
over the UART to the PC.

More details about this Test Application can be found in WICED Studio from Cypress https://www.cypress.com/products/wiced-software



Toolchain supported
===================
- IAR embedded Workbench  8.32.3
- GCC ARM Embedded  8.2.1
- MCUXpresso  11.0.0

Hardware requirements
=====================
- Micro USB cable
- evkmimxrt1060 board
- Personal Computer
- Murata 1DX M.2 module
- Murata uSD M.2 Adapter

Board settings
==============
Connect the board with PC by using USB/UART converter:
- board uart RX (pin 1 on J22) - connect it to TX pin on converter
- board uart TX (pin 2 on J22) - connect it to RX pin on converter
- board GND (pin 7 on J24)

Prepare the Demo
================

Running the demo
================
1.  Connect a USB cable between the PC host and the OpenSDA port on the target board.

2.  Open a serial terminal on PC for serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
    
3.  Connect the WiFi module, details are in section: Board settings.

4.  Download the program to the target board with the CMSIS-DAP or J-Link debugger.

5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

6.  Connect the board with PC by USB/UART converter, details for connection are in section: Board settings.

7.  Open a command prompt and navigate to the middleware\wifi_wiced\libraries\test\wl_tool\ direcotry.

8.  Enter the following command at the command prompt to verify that the application is working:
      wl<chip_name>.exe --serial 99 ver
      Where 99 corresponds to the COM port number of the DUT (COM port number of USB/UART converter, can be found in windows device manager)

Example of using the wl utility from command prompt:
    C:\mcu-sdk-2.0\middleware\wifi_wiced\libraries\test\wl_tool>wl4343WA1.exe --serial 41 ver
    7.45 RC41.0
    wl0: Apr 30 2018 04:13:29 version 7.45.98.50 (r688715 CY WLTEST) FWID 01-68e4a7a6


The log below shows the output of the demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Initializing WiFi Connection... 
WLAN MAC Address : A0:C9:A0:3D:F9:2F
WLAN Firmware    : wl0: Apr 30 2018 04:13:29 version 7.45.98.50 (r688715 CY WLTEST) FWID 01-68e4a7a6
WLAN CLM         : API: 12.2 Data: 9.10.39 Compiler: 1.29.4 ClmImport: 1.36.3 Creation: 2018-04-11 22:31:04 
Successfully Initialized WiFi Connection 
Starting WL server
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Customization options
=====================

