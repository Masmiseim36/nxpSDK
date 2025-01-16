Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT1020-EVK board
- Personal Computer

Board settings
==============
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
The log below shows the output of the xbar aoi demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
XBAR and AOI Demo: Start...

Then change CMP analog input.
    - CMP1_IN0 (J17-7) connected to VCC =3.3V (J19-8): XBAR interrupt occurred
    - CMP1_IN0 (J17-7) connected to GND (J19-7): XBAT interrupt stopped

XBAR interrupt occurred

XBAR interrupt occurred
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
