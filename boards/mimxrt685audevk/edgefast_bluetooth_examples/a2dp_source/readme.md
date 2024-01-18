Overview
========
Application demonstrating how to use the a2dp source feature.


SDK version
===========
- Version: 2.15.000

Toolchain supported
===================
- MCUXpresso  11.8.0
- IAR embedded Workbench  9.40.1
- Keil MDK  5.38.1
- GCC ARM Embedded  12.2

Hardware requirements
=====================
- Micro USB cable
- mimxrt685audevk board
- Personal Computer
- One of the following modules:
  - WIFI_IW416_BOARD_MURATA_1XK_M2

Board settings
==============
Before building the example application select Wi-Fi module macro in the app_config.h. (see #define WIFI_<SoC Name>_BOARD_<Module Name>).
If you want use the WIFI_IW416_BOARD_MURATA_1XK_M2, please change the macro to WIFI_IW416_BOARD_MURATA_1XK_M2.

Murata Solution Board settings
Murata uSD-M.2 adapter resource page: https://www.murata.com/en-us/products/connectivitymodule/wi-fi-bluetooth/overview/lineup/usd-m2-adapter
Murata uSD-M.2 adapter datasheet: https://www.murata.com/-/media/webrenewal/products/connectivitymodule/asset/pub/rfm/data/usd-m2_revb1.ashx
Embedded Artists M.2 module resource page: https://www.embeddedartists.com/m2
Embedded Artists 1XK module datasheet: https://www.embeddedartists.com/doc/ds/1XK_M2_Datasheet.pdf
Embedded Artists 1ZM module datasheet: https://www.embeddedartists.com/doc/ds/1ZM_M2_Datasheet.pdf

Jumper settings for WIFI_IW416_BOARD_MURATA_1XK_M2:
  - J41 : from position (1-2) to position (2-3)
  - move R300-R305 from position A(2-1) to position B(2-3)

The following pins between the mimxrt685audevk board and Murata uSD-M.2 Adapter with 1XK M.2 Module are connected using male-to-female jumper cables:

------------------------------------------------------------------------------------------------
PIN NAME         | uSD-M.2 Adapter |   I.MXRT685    |  PIN NAME OF RT685 | GPIO NAME OF RT685
------------------------------------------------------------------------------------------------
BT_UART_TXD_HOST |  J45(pin 32)  	   |   U18B(G17)   |    USART5_TXD      | FC5_TXD_SCL_MISO_WS
BT_UART_RXD_HOST |  J45(pin 22)  	   |   U18B(J16)   |    USART5_RXD      | FC5_RXD_SDA_MOSI_DATA
BT_UART_RTS_HOST |  J45(pin 36)  	   |   U18B(J15)   |    USART5_RTS      | FC5_RTS_SCL_SSEL1
BT_UART_CTS_HOST |  J45(pin 34)  	   |   U18B(J17)   |    USART5_CTS      | FC5_CTS_SDA_SSEL0
------------------------------------------------------------------------------------------------

Note:
After downloaded binary into qspiflash and boot from qspiflash directly, 
please reset the board by pressing SW3 or power off and on the board to run the application.
Prepare the Demo
================

1.  Open example's project and build it.

2.  Connect a USB cable between the PC host and the OpenSDA USB port on the target board.

3.  Provide 5V voltage for the target board.

4.  Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control

5.  Download the program to the target board.

6.  Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
The log below shows the output of the example in the terminal window.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
USB Host stack successfully initialized
Bluetooth initialized

SHELL build: Mar  2 2021
Copyright  2021  NXP

>> 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Procedures to run
1 input "help" to show command list
2 input "bt discover" to discover connctable bluetooth devices
3 input "bt connect [index]" to create basic bluetooth connection with the discovered device
4 the music start playing after connection success.
