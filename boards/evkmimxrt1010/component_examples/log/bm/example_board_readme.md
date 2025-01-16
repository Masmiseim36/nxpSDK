Hardware requirements
=====================
- Mini/micro USB cable
- EVK-MIMXRT1010 board
- Personal Computer

Board settings
============
No special settings are required.

Prepare the Demo
===============
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
SHELL build: Sep 27 2020
Copyright  2020  NXP

LOG SHELL>> installbackend debugconsole
LOG SHELL>> log fatal
           0: FATAL log_main.c:83:This is "fatal" log message

LOG SHELL>> log error
           0: ERROR log_main.c:87:This is "error" log message

LOG SHELL>> log warning
           0: WARN  log_main.c:91:This is "warning" log message

LOG SHELL>> log info
           0: INFO  log_main.c:95:This is "info" log message

LOG SHELL>> log debug
           0: DEBUG log_main.c:99:This is "debug" log message

LOG SHELL>> log trace
           0: TRACE log_main.c:103:This is "trace" log message

LOG SHELL>> log test
           0: ERROR log_main.c:107:The input arguement "test" is not valid

LOG SHELL>> uninstallbackend debugconsole
LOG SHELL>> 
~~~~~~~~~~~~~~~~~~~~~
Note: The shell information "SHELL build: Sep 27 2020" may be different, which depends on the compile date.


Note:
To debug in qspiflash, following steps are needed:
1. Select the flash target and compile.
2. Set the SW8: 1 off 2 off 3 on 4 off, then power on the board and connect USB cable to J41.
3. Start debugging in IDE.
   - Keil: Click "Download (F8)" to program the image to qspiflash first then clicking "Start/Stop Debug Session (Ctrl+F5)" to start debugging.
