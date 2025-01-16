Hardware requirements
=====================
- Micro USB cable
- evkcmimxrt1060 board
- Personal Computer
- One of the following modules:
  - Embedded Artists 1XK M.2 Module (EAR00385) - direct M2 connection.
  - Embedded Artists 1ZM M.2 Module (EAR00364) - direct M2 connection.
  - Embedded Artists 2EL M.2 Module (Rev-A1) - direct M2 connection.

Board settings
==============
Before building the example application select Wi-Fi module macro in the app_bluetooth_config.h. (see #define WIFI_<SoC Name>_BOARD_<Module Name>).
If you want to use Embedded Artists 1XK M.2 Module(EAR00385), please change the macro to WIFI_IW416_BOARD_MURATA_1XK_M2.
If you want to use Embedded Artists 1ZM M.2 Module(EAR00364), please change the macro to WIFI_88W8987_BOARD_MURATA_1ZM_M2.
If you want to use Embedded Artists 1ZM M.2 Module(Rev-A1), please change the macro to WIFI_IW612_BOARD_MURATA_2EL_M2.

Jumper settings for RT1060-EVKC (enables external 5V supply):
========================================================
remove  J40 5-6
connect J40 1-2
connect J45 with external power(controlled by SW6)

BT Independent Reset OOB Trigger For 1XK/1ZM/2EL:
	- Connect Fly-Wire between J16.7 and J108.3.
	- J108 is routed on M2.P48 which internally routed on IR GPIO[15] of Controller 1XK/1ZM.
	- For 2EL-M2, No fly-wire connection required.

Murata Solution Board settings
==============================
Embedded Artists 1XK module datasheet: https://www.embeddedartists.com/doc/ds/1XK_M2_Datasheet.pdf
Embedded Artists 1ZM module datasheet: https://www.embeddedartists.com/doc/ds/1ZM_M2_Datasheet.pdf
Embedded Artists 2EL module datasheet: https://www.embeddedartists.com/doc/ds/2EL_M2_Datasheet.pdf

The hardware should be reworked according to the hardware rework guide for evkcmimxrt1060 and Murata 1XK M.2 Adapter in document Hardware Rework Guide for EdgeFast BT PAL.

The hardware should be reworked according to the hardware rework guide for evkcmimxrt1060 and Murata 1ZM M.2 Adapter in document Hardware Rework Guide for EdgeFast BT PAL.

Note:
To ensure that the LITTLEFS flash region has been cleaned,
all flash sectors need to be erased before downloading example code.
After downloaded binary into qspiflash and boot from qspiflash directly,
please reset the board by pressing SW7 or power off and on the board to run the application.
