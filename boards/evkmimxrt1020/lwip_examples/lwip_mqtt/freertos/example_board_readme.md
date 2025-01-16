Hardware requirements
===================
- Mini/micro USB cable
- Network cable RJ45 standard
- EVK-MIMXRT1020 board
- Personal Computer

Board settings
============
No special settings are required.


Note:
To debug in qspiflash, following steps are needed:
1. Select the flash target and compile.
2. Set the SW8: 1 off 2 off 3 on 4 off, then power on the board and connect USB cable to J23.
3. Start debugging in IDE.
   - Keil: Click "Download (F8)" to program the image to qspiflash first then clicking "Start/Stop Debug Session (Ctrl+F5)" to start debugging.
