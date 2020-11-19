Overview
========
The XBAR AOI demo application demonstrates the usage of the XBAR, AOI peripheral to combine interrupt
from CMP and PIT. If the button is pressed(CMP interrupt) and the PIT periodic interrupt occurs a message is printed.

Toolchain supported
===================
- IAR embedded Workbench  8.50.6
- Keil MDK  5.31
- GCC ARM Embedded  9.2.1
- MCUXpresso  11.2.1

Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT1024-EVK board
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
The log below shows the output of the xbar aoi demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
XBAR and AOI Demo: Start...

Then change CMP analog input.
    - CMP1_IN0 (J17-14) connected to VCC =3.3V (J19-16): XBAR interrupt occurred
    - CMP1_IN0 (J17-14) connected to GND (J19-14): XBAT interrupt stopped

XBAR interrupt occurred

XBAR interrupt occurred
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
