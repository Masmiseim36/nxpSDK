Overview
========
The RTWDOG Example project is to demonstrate usage of the KSDK rtwdog driver.
In this example, fast testing is first implemented to test the rtwdog.
After this, refreshing the watchdog in None-window mode and window mode is executed.
Note rtwdog is disabled in SystemInit function which means rtwdog is disabled
after chip emerges from reset.

Toolchain supported
===================
- IAR embedded Workbench  8.32.3
- Keil MDK  5.26
- GCC ARM Embedded  7.3.1
- MCUXpresso  10.3.1

Hardware requirements
=====================
- Mini/micro USB cable
- EVK-MIMXRT1015 board
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
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the example runs successfully, the following message is displayed in the terminal:

~~~~~~~~~~~~~~~~~~~~~
----- Fast test starts -----
Low Byte test starts
Waiting for timeout reset
Low Byte test succeeded
----- High Byte test starts -----
Waiting for timeout reset
High Byte test succeeded
----- The end of RTWDOG fast test -----

----- Refresh test start -----
----- None-window mode -----
Refresh rtwdog 1 time
Refresh rtwdog 2 time
Refresh rtwdog 3 time
Refresh rtwdog 4 time
Refresh rtwdog 5 time
Refresh rtwdog 6 time
Waiting for time out reset
None-window mode reset succeeded
----- Window mode -----
Refresh rtwdog 1 time
Refresh rtwdog 2 time
Refresh rtwdog 3 time
Waiting for time out reset
Window mode reset succeeded
~~~~~~~~~~~~~~~~~~~~~

Note:

After disable the global interrupts, system needs to be finished configuration within a specified time,
otherwise, it will be block during rtwdog_init.

So, when there is no print in flexspi_nor_debug target or degug taget(MCUx IDE), please improve the level of optimization.


Note:
To download binary into qspiflash and boot from qspiflash directly, following steps are needed:
1. Compile flash target of the project, and get the binaray file "rtwdog.bin".
3. Set the SW8: 1 off 2 off 3 on 4 off, then power on the board and connect USB cable to J41.
4. Drop the binaray into disk "RT1015-EVK" on PC.
5. Wait for the disk disappear and appear again which will take couple of seconds.
7. Reset the board by pressing SW3 or power off and on the board. 

Note:
To debug in qspiflash, following steps are needed:
1. Select the flash target and compile.
3. Set the SW8: 1 off 2 off 3 on 4 off, then power on the board and connect USB cable to J41.
4. Start debugging in IDE.
   - Keil: Click "Download (F8)" to program the image to qspiflash first then clicking "Start/Stop Debug Session (Ctrl+F5)" to start debugging.
Customization options
=====================

