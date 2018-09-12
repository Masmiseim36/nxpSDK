Overview
========
The WDOG Example project is to demonstrate usage of the KSDK wdog driver.
In this example,implemented to test the wdog.
And then after 10 times of refreshing the watchdog, a timeout reset is generated.
We also try to refresh out of window to trigger reset after 10 times of refreshing.
Please notice that because WDOG control registers are write-once only, so the 
WDOG_Init function and the WDOG_Disable function can be called only once after reset.

Toolchain supported
===================
- Keil MDK 5.25
- IAR embedded Workbench 8.30.2
- GCC ARM Embedded 7-2017-q4-major
- MCUXpresso10.2.1

Hardware requirements
=====================
- Mini/micro USB cable
- EVK-MIMXRT1060 board
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
These instructions are displayed/shown on the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~
--- Start Wdog Ultra test ---
--- wdog Init done---
--- Refresh wdog 1 time ---
--- Refresh wdog 2 time ---
--- Refresh wdog 3 time ---
--- Refresh wdog 4 time ---
--- Refresh wdog 5 time ---
--- Refresh wdog 6 time ---
--- Refresh wdog 7 time ---
--- Refresh wdog 8 time ---
--- Refresh wdog 9 time ---
--- Refresh wdog 10 time ---

End of Wdog example!
~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

