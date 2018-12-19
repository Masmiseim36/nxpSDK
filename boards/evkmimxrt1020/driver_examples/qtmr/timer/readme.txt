Overview
========

The QTMR project is a simple demonstration program of the SDK QTMR driver to use QTMR as a timer.
The quad-timer module provides four timer channels with a variety of controls affecting both individual
and multi-channel features. 
It sets up the QTMR hardware block to trigger an interrupt every 50 millisecond.
When chaining the first channel and the second channel, Timer interrupt occur every 10 second.
When the QTMR interrupt is triggered a message a printed on the UART terminal.
Hardware requirements
=====================
- Mini/micro USB cable
- EVK-MIMXRT1020 board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board. 
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
When the demo runs successfully, the following message is displayed in the terminal:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*********QUADTIMER EXAMPLE START*********
****Timer use-case, 50 millisecond tick.****

 Timer interrupt has occured !
 Timer interrupt has occured !
 Timer interrupt has occured !
 Timer interrupt has occured !
 Timer interrupt has occured !
 Timer interrupt has occured !
 Timer interrupt has occured !
 Timer interrupt has occured !
 Timer interrupt has occured !
 Timer interrupt has occured !
****Chain Timer use-case, 10 second tick.****

 Timer interrupt has occured !
 Timer interrupt has occured !
 Timer interrupt has occured !
 Timer interrupt has occured !
 Timer interrupt has occured !
*********QUADTIMER EXAMPLE END.*********
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Note:
To debug in qspiflash, following steps are needed:
1. Select the flash target and compile.
3. Set the SW8: 1 off 2 off 3 on 4 off, then power on the board and connect USB cable to J23.
4. Start debugging in IDE.
   - Keil: Click "Download (F8)" to program the image to qspiflash first then clicking "Start/Stop Debug Session (Ctrl+F5)" to start debugging.

Toolchain supported
===================
- IAR embedded Workbench  8.32.1
- Keil MDK  5.26
- GCC ARM Embedded  7.3.1
- MCUXpresso 10.3.0

