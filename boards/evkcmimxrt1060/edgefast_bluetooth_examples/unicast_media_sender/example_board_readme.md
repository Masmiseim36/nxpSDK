Hardware requirements
=====================
- Micro USB cable
- evkcmimxrt1060 board
- Personal Computer
- Embedded Artists 2EL M.2 Module (Rev-A1) - direct M2 connection.

Board settings
==============
Before building the example application select Wi-Fi module macro in the app_bluetooth_config.h. (see #define WIFI_<SoC Name>_BOARD_<Module Name>).
If you want to use Embedded Artists 2EL M.2 Module(Rev-A1), please change the macro to WIFI_IW612_BOARD_MURATA_2EL_M2.

Jumper settings for RT1060-EVKC (enables external 5V supply):
========================================================
remove  J40 5-6
connect J40 1-2
connect J45 with external power(controlled by SW6)
connect ENET_MDC(GPT2_CAP2) with 2EL's GPIO_27(Sync Signal)

Murata Solution Board settings
==============================
Embedded Artists 2EL module datasheet: https://www.embeddedartists.com/doc/ds/2EL_M2_Datasheet.pdf

The hardware should be reworked according to the hardware rework guide for evkcmimxrt1060 and Murata 1ZM M.2 Adapter in document Hardware Rework Guide for EdgeFast BT PAL.

Note:
To ensure that the LITTLEFS flash region has been cleaned,
all flash sectors need to be erased before downloading example code.
After downloaded binary into qspiflash and boot from qspiflash directly,
please reset the board by pressing SW7 or power off and on the board to run the application.
