Overview
========


Toolchain supported
===================
- Keil MDK 5.24a
- IAR embedded Workbench 8.22.2
- GCC ARM Embedded 7-2017-q4-major
- MCUXpresso10.2.0

Hardware requirements
=====================
- Mini/micro USB cable
- EVK-MIMXRT1020 board
- GT202 Adaptor V1.04
- Personal Computer

Board settings
==============
Plug GT202 Adaptor board to FRDM stackable headers (J1, J2, J3, J4).
Remove the resistor R137.
Please use external power supply (J2), USB may not be sufficient.


Note:
To debug in qspiflash, following steps are needed:
1. Select the flash target and compile.
3. Set the SW8: 1 off 2 off 3 on 4 off, then power on the board and connect USB cable to J23.
4. Start debugging in IDE.
   - Keil: Click "Download (F8)" to program the image to qspiflash first then clicking "Start/Stop Debug Session (Ctrl+F5)" to start debugging.
Prepare the Demo
================


Running the demo
================
1. Download the program to the target board.
2. When the application runs, go to "fw/" directory and execute from 
   command line "host.exe COM59 flashotp_3_3_5.bin 845dd7490e4c -b115200"
   where:
    COM59               - is your PC/board serial interface
    flashotp_3_3_5.bin  - is a path to firmware binary
    845dd7490e4c        - is a MAC addrees of GT202 module
    -bb115200           - is a communication buadrate

IMPORTANT: Do not try to update firmware on Silex shield !!!

NOTE: host.exe is taken from QUALCOMM MQX installer package
NOTE: We have discovered some issues using Jlink CMSIS-DAP Serial for FRDMK64 Rev.E.
      If the transmission stops after few kBytes, we recommend to use DAPLINK.
NOTE: host.exe cannot resolve COM port with number greater-than 99

Customization options
=====================

