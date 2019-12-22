Overview
========
The OTA(Over-the-Air) bootloader is a secondary bootloader resides in external memory which supports updating and booting the application.

Hardware requirements
=====================
- Mini/micro USB cable
- EVK-MIMXRT1060 board
- Personal Computer

Board settings
============
No special settings are required.

Prepare
===============
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Download the program to the target board.
3.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Note:
To download binary into hyper flash in MDK and boot from hyperflash directly, following steps are needed:
1. Select the target flexspi_nor_release.
2. Compile the project.
3. Press F8 or click the download button, to program the application into hyper flash.
4. Set the SW7: 1 off 2 on 3 on 4 off, then power on the board
5. After power on the baord, program has already started to run, reset SW4 is recommended.

Note:
To debug in hyper flash in MDK, following steps are needed:
1. Select the target flexspi_nor_release.
2. Compile the project.
3. Press F8 or click the download button, to program the application into hyper flash.
4. Set the SW7: 1 off 2 on 3 on 4 off, then power on the board
5. Push SW4 to reset.
6. Start to debug.

Note:
To debug in hyper flash in MCUXpresso, following steps are needed:
1. Set the SW7: 1 off 2 on 3 on 4 off, then power on the board
2. Compile the project.
3. Start to debug.

Known issue:
Currently, this example can't be downloaded using Jlink, for no supported flash algorithm.
