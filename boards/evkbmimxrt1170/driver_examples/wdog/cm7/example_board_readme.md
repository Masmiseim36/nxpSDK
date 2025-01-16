Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT1170-EVKB board
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
===============
These instructions are displayed/shown on the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~
******** System Start ********
System reset by: Power On Reset!

- 1.Testing System reset by software trigger.
******** System Start ********
System reset by: Software Reset!

- 2.Testing system reset by WDOG timeout.
--- wdog Init done---

******** System Start ********
System reset by: Time Out Reset!

- 3.Test the WDOG refresh function by using interrupt.
--- wdog Init done---

WDOG has be refreshed!
WDOG has be refreshed!
WDOG has be refreshed!

~~~~~~~~~~~~~~~~~~~~~