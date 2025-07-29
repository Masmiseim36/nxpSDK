Hardware requirements
=====================
- Mini/micro USB cable
- RT700 board
- Personal Computer

Board settings
============
In default, XSPI2 is used. No special board settings are required.
The driver example and board schematic also support change to use XSPI1. Please refer to the following steps:
1. In app.h file, change the following macro definition:  
    `#define DEMO_USE_XSPI2 0`  
    `#define DEMO_USE_XSPI1 1`
2. Change default jumper of JP45 from 2-3 to 1-2.


Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the MCU-Link USB port on the target board.
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
When the example runs successfully, you can see the similar information from the terminal as below.
~~~~~~~~~~~~~~~~~~~~~
XSPI edma example started!
EDMA Command Read/Write data successfully at all address range !
~~~~~~~~~~~~~~~~~~~~~

