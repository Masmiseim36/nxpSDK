Overview
========
This example demonstrates the HFP Ag basic functionality, currently support simulate an incoming call, and the call could be answered and ended.
The HFP Ag can connected a HFP HF device like headphone or device running HFP HF device.


Toolchain supported
===================
- MCUXpresso  11.7.0
- IAR embedded Workbench  9.32.1
- Keil MDK  5.37
- GCC ARM Embedded  10.3.1

Hardware requirements
=====================
- Micro USB cable
- evkmimxrt595 board
- Personal Computer
- USB to serial converter
- One of the following modules:
  - AzureWave AW-CM358MA.M2
  - AzureWave AW-CM510MA.M2
  - Embedded Artists 1XK M.2 Module (EAR00385)
  - Embedded Artists 1ZM M.2 Module (EAR00364)


Board settings
==============
Before building the example application select Wi-Fi module macro in the app_config.h. (see #define WIFI_<SoC Name>_BOARD_<Module Name>).
If you want to use AzureWave WIFI_IW416_BOARD_AW_AM510MA, please change the macro to WIFI_IW416_BOARD_AW_AM510MA.
If you want to use AzureWave WIFI_88W8987_BOARD_AW_CM358MA, please change the macro to WIFI_88W8987_BOARD_AW_CM358MA.
If you want to use Embedded Artists Type 1XK module (EAR00385), please change the macro to WIFI_IW416_BOARD_MURATA_1XK_M2.
If you want to use Embedded Artists Type 1ZM module (EAR00364), please change the macro to WIFI_88W8987_BOARD_MURATA_1ZM_M2.


Jumper settings for RT595 (enables external 5V supply):
connect J39 with external power
JP4 1-2
J27 1 - TX of USB to serial converter
J27 2 - RX of USB to serial converter
JP7 1-2
JP8 1-2
JP27 1-2
JP28 1-2
JP29 1-2

Debug console UART is configured to use pins of J27, connect the board with PC by USB/UART converter:
- board UART RX (pin 1 on J27) - connect to TX pin on converter
- board UART TX (pin 2 on J27) - connect to RX pin on converter
- board GND (pin 7 on J29) - connect to GND pin on converter

Murata Solution Board settings
Embedded Artists M.2 module resource page: https://www.embeddedartists.com/m2
Embedded Artists 1XK module datasheet: https://www.embeddedartists.com/doc/ds/1XK_M2_Datasheet.pdf
Embedded Artists 1ZM module datasheet: https://www.embeddedartists.com/doc/ds/1ZM_M2_Datasheet.pdf

AzureWave Solution Board settings
The hardware should be reworked according to the Hardware Rework Guide for MIMXRT595-EVK and AW-AM510MA in document Hardware Rework Guide for EdgeFast BT PAL.

The hardware should be reworked according to the Hardware Rework Guide for MIMXRT595-EVK and AW-CM358MA in document Hardware Rework Guide for EdgeFast BT PAL.



Note:
After downloaded binary into qspiflash and boot from qspiflash directly, 
please reset the board by pressing SW3 or power off and on the board to run the application.
Prepare the Demo
================

1.  Open example's project and build it.

2.  Connect a USB cable between the PC host and the OpenSDA USB port on the target board.

3.  Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control

4.  Download the program to the target board.

5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
The log below shows the output of the example in the terminal window. 
USB Host stack successfully initialized
Bluetooth initialized

SHELL build: Mar  1 2021
Copyright  2020  NXP

>>
the bellow commands have been supported:
"bt": BT related function
  USAGE: bt [discover|connect|disconnect|delete]
    discover             start to find BT devices
    connect              connect to the device that is found, for example: bt connect n (from 1)
    openaudio            open audio connection without calls
    closeaudio           close audio connection without calls 
    sincall              start an incoming call.
    aincall              accept the call.
    eincall              end an call.
    set_tag              set phone num tag, for example: bt set_tag 123456789.
    select_codec         codec select for codec Negotiation, for example: bt select_codec 2, it will select the codec 2 as codec.
    set_mic_volume       update mic Volume, for example: bt set_mic_volume 14.
    set_speaker_volume   update Speaker Volume, for example: bt set_speaker_volume 14.
    stwcincall           start multiple an incoming call.
    disconnect           disconnect current connection.
    delete               delete all devices. Ensure to disconnect the HCI link connection with the peer device before attempting to delete the bonding information.
1) "discover" start to find BT devices, it will list all device can be found, usage
2) "connect" is used to connect to the device that is found, for example: bt connect n (from 1), usage :
   bt connect 1
3) "openaudio" is used to open audio connection without calls
4) "closeaudio" is used to close audio connection without calls 
5) "sincall" is used to start an incoming call
6) "aincall" is used to accept an incoming call
7) "eincall" is used to end or reject an incoming call
8) "set_tag" is used to set phone num tag, for example: bt set_tag 123456789
9) "select_codec" is used to  codec select for codec Negotiation, for example: bt select_codec 2, it will select the codec 2 as codec, usage:
   bt select_codec 2
10) "set_mic_volume" is used to set mic volume, the value  is from 1 to 15, usage as:
    bt set_mic_volume 8
11) "set_speaker_volume" is used to set speaker volume, the value is from 1 to 15, usage as:
    bt set_speaker_volume 8
12) "stwcincall"  to start multiple an incoming call, need run "sincall" is used to start an incoming call before run the command
13) "disconnect"  to disconnect current connection
14) "delete" is used to delete all devices. Ensure to disconnect the HCI link connection with the peer device before attempting to delete the bonding information.

Note:
There is a short noise can be heard at headset at the begin audio streaming when in running HFP Ag . 
The codec power on pop noise cannot eliminate.

