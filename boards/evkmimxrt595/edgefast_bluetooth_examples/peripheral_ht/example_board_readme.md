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
  - Embedded Artists 2EL M.2 Module (Rev-A1) - direct M2 connection.

Board settings
==============
Before building the example application select Wi-Fi module macro in the app_bluetooth_config.h. (see #define WIFI_<SoC Name>_BOARD_<Module Name>).
If you want to use AzureWave WIFI_IW416_BOARD_AW_AM510MA, please change the macro to WIFI_IW416_BOARD_AW_AM510MA.
If you want to use AzureWave WIFI_88W8987_BOARD_AW_CM358MA, please change the macro to WIFI_88W8987_BOARD_AW_CM358MA.
If you want to use Embedded Artists Type 1XK module (EAR00385), please change the macro to WIFI_IW416_BOARD_MURATA_1XK_M2.
If you want to use Embedded Artists Type 1ZM module (EAR00364), please change the macro to WIFI_88W8987_BOARD_MURATA_1ZM_M2.
If you want to use Embedded Artists Type 2EL module (Rev-A1), please change the macro to WIFI_IW612_BOARD_MURATA_2EL_M2.


Jumper settings for RT595 (enables external 5V supply):
=======================================================
connect J39 with external power
JP4 1-2
J27 1 - TX of USB to serial converter
J27 2 - RX of USB to serial converter


Debug console UART is configured to use pins of J27, connect the board with PC by USB/UART converter:
- board UART RX (pin 1 on J27) - connect to TX pin on converter
- board UART TX (pin 2 on J27) - connect to RX pin on converter
- board GND (pin 7 on J29) - connect to GND pin on converter

Murata Solution Board settings
==============================
Embedded Artists M.2 module resource page: https://www.embeddedartists.com/m2
Embedded Artists 1XK module datasheet: https://www.embeddedartists.com/doc/ds/1XK_M2_Datasheet.pdf
Embedded Artists 1ZM module datasheet: https://www.embeddedartists.com/doc/ds/1ZM_M2_Datasheet.pdf
Embedded Artists 2EL module datasheet: https://www.embeddedartists.com/doc/ds/2EL_M2_Datasheet.pdf

AzureWave Solution Board settings
=================================
The hardware should be reworked according to the Hardware Rework Guide for MIMXRT595-EVK and AW-AM510MA in document Hardware Rework Guide for EdgeFast BT PAL.
The hardware should be reworked according to the Hardware Rework Guide for MIMXRT595-EVK and AW-CM358MA in document Hardware Rework Guide for EdgeFast BT PAL.

Low power feature specific settings
=================================
Low power feature is supported on the following modules: 
  - Embedded Artists 1XK M.2 Module (Rev-A)
  - Embedded Artists 2EL M.2 Module (Rev-A1)
For hardware rework, please refer to 'Hardware Rework Guide for EdgeFast BT PAL'.
To enable low power feature, please set APP_LOWPOWER_ENABLED as 1 in the app_bluetooth_config.h.

Note:
To ensure that the LITTLEFS flash region has been cleaned,
all flash sectors need to be erased before downloading example code.
After downloaded binary into qspiflash and boot from qspiflash directly,
please reset the board by pressing SW3 or power off and on the board to run the application.
For K32W061 controller, it's not supported. Code supporting K32W061 is kept, but isn't verified.
