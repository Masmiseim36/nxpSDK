Overview
========
The WDOG Example project is to demonstrate usage of the KSDK wdog driver.
In this example,quick test is first implemented to test the wdog.
And then after 10 times of refreshing the watchdog in None-window mode, a timeout reset is generated.
We also try to refresh out of window to trigger reset after 10 times of refreshing in Window mode.

Toolchain supported
===================
- IAR embedded Workbench 7.80.4
- Keil MDK 5.21a
- GCC ARM Embedded 2016-5.4-q3
- Kinetis Development Studio IDE 3.2.0
- MCUXpresso0.8

Hardware requirements
=====================
- J-Link ARM
- TWR-KM34Z75M board
- Mini USB cable
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

When the example runs successfully, the following message is displayed in the terminal:

~~~~~~~~~~~~~~~~~~~~~
--- Quick test ---
--- Quick test done ---

--- None-window mode refresh test start---
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
--- None-window mode refresh test done ---

--- Window mode refresh test start---
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
--- Window mode refresh test done---

End of Wdog example!
~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

