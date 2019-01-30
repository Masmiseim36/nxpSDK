Overview
========
The XBAR AOI demo application demonstrates the usage of the XBAR, AOI peripheral to combine interrupt
from CMP and PIT. If the button is pressed(CMP interrupt) and the PIT periodic interrupt occurs a message is printed.
Hardware requirements
=====================
- Mini/micro USB cable
- TWR-KV46F150M board
- Personal Computer

Board settings
==============
- To use UART on board, make sure that J505-3 connects to J505-4, J506-3 connects to J506-4
- Shorted J4 1-2 to use SW1

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
Press SW1 to run demo.
The log below shows the output of the xbar aoi demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
XBAR and AOI Demo: Start...

XBAR interrupt occurred

XBAR interrupt occurred
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Toolchain supported
===================
- IAR embedded Workbench  8.32.1
- Keil MDK  5.26
- GCC ARM Embedded  7.3.1
- MCUXpresso 10.3.0

