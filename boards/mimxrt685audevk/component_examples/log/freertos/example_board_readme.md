Hardware requirements
=====================
- Micro USB cable
- MIMXRT685-AUD-EVK board
- Personal Computer

Board settings
============

Prepare the Demo
===============
1.  Connect a micro USB cable between the PC host and the CMSIS DAP USB port (J5) on the board
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Launch the debugger in your IDE to begin running the demo.

Running the demo
================
The log below shows the output of the shell demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~
SHELL build: Sep 27 2020
 * Copyright 2021 NXP

LOG SHELL>> installbackend debugconsole
LOG SHELL>> log error
       22350: ERROR log_main.c:104:This is "error" log message

LOG SHELL>> log fatal
       27900: FATAL log_main.c:100:This is "fatal" log message

LOG SHELL>> log warning
       35430: WARN  log_main.c:108:This is "warning" log message

LOG SHELL>> log info
       38585: INFO  log_main.c:112:This is "info" log message

LOG SHELL>> log debug
       41330: DEBUG log_main.c:116:This is "debug" log message

LOG SHELL>> log trace
       45090: TRACE log_main.c:120:This is "trace" log message

LOG SHELL>> log test
       49835: ERROR log_main.c:124:The input arguement "test" is not valid

LOG SHELL>> uninstallbackend debugconsole
LOG SHELL>> 
~~~~~~~~~~~~~~~~~~~~~
Note: The shell information "SHELL build: Sep 27 2020" may be different, which depends on the compile date.
