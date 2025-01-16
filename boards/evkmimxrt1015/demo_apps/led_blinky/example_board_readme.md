Hardware requirements
=====================
- Mini/micro USB cable
- EVK-MIMXRT1015 board
- Personal Computer

Board settings
============
No special settings are required.

Prepare the Demo
===============
1. Connect a USB cable between the host PC and the OpenSDA USB port on the target board. 
2. Download the program to the target board.
3. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the demo runs successfully, you will find the LED is blinking.

Note:
To debug in qspiflash, following steps are needed:
1. Select the flash target and compile.
3. Set the SW8: 1 off 2 off 3 on 4 off, then power on the board and connect USB cable to J41.
4. Start debugging in IDE.
   - Keil: Click "Download (F8)" to program the image to qspiflash first then clicking "Start/Stop Debug Session (Ctrl+F5)" to start debugging.
