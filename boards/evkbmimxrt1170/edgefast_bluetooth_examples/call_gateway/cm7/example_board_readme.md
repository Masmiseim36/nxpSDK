Hardware requirements
=====================
- Micro USB cable
- evkbmimxrt1170 board
- Personal Computer
- Embedded Artists 2EL M.2 Module (Rev-A1) - direct M2 connection.

Jumper settings for RT1170-EVKB (enables external 5V supply):
========================================================
remove  J38 5-6
connect J38 1-2
connect J43 with external power(controlled by SW5)

Murata Solution Board settings
==============================
Embedded Artists M.2 module resource page: https://www.embeddedartists.com/m2
Embedded Artists 2EL module datasheet: https://www.embeddedartists.com/doc/ds/2EL_M2_Datasheet.pdf

The hardware rework for MIMXRT1170-EVKB and Murata 2EL M.2 Adapter is same as MIMXRT1170-EVKB and Murata 1XK M.2 Adapter.
Note:
After downloaded binary into qspiflash and boot from qspiflash directly,
please reset the board by pressing SW4 or power off and on the board to run the application.
