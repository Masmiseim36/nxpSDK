Hardware requirements
=====================
- Two Micro USB cable
- EVK-MIMXRT1010 board
- Personal Computer

Board settings
============
No special settings are required.

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board for downloading
    the program and supply power for the board, connect another USB cable between host PC and USB port
	on the target board.
2.  Download the program to the target board.
3.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.
4.  Open a serial terminal(like putty) to communicate with the board via virtual com.

Running the demo
================
The log below shows the output of this demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
hello world.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Terminal will echo back the received characters.

Note:
To debug in qspiflash, following steps are needed:
1. Select the flash target and compile.
2. Set the SW8: 1 off 2 off 3 on 4 off, then power on the board and connect USB cable to J41.
3. Start debugging in IDE.
   - Keil: Click "Download (F8)" to program the image to qspiflash first then clicking "Start/Stop Debug Session (Ctrl+F5)" to start debugging.

Note:
When the USB serial port is emuerated and termial is opened, the printf has been executed, so the message cannot be displayed.
Please use putty,sscom and other terminal that support data caching replace it.