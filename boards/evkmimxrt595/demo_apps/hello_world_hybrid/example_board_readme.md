Hardware requirements
=====================
- Micro USB cable
- EVK-MIMXRT595 board
- Personal Computer

Board settings
============


Prepare the Demo
===============
1.  Connect a micro USB cable between the PC host and the CMSIS DAP USB port (J40) on the board
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Launch the debugger in your IDE to begin running the demo.
!!!Note!!! 
When debugging this demo using IAR, need force the IAR IDE to use hardware breakpoints as
the software breakpoints will be overwritten during the startup code relocates code from flash to RAM.
The steps to force the IAR IDE to use hardware breakpoints: Change the "Default breakpoint type" setting to 
"Hardware" under the tab of "Options" -> "Debugger"-> "CMSIS DAP" or "J-Link/J-Trace" basing on the debugger being used.

