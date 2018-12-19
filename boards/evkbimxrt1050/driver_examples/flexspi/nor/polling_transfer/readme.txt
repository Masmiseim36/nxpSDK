Overview
========
The flexspi_nor_polling_transfer example shows how to use flexspi driver with polling:

In this example, flexspi will send data and operate the external Nor flash connected with FLEXSPI. Some simple flash command will
be executed, such as Write Enable, Erase sector, Program page.
Example will first erase the sector and program a page into the flash, at last check if the data in flash is correct.
Hardware requirements
=====================
- Mini/micro USB cable
- EVKB-IMXRT1050 board
- Personal Computer

Board settings
==============
The board enables hyper flash (U19) by default. To enable QSPI flash (U33), some hardware rework is needed:
Mount R153,R154,R155,R156,R157,R158 and DNP R356,R361,R362,R363,R364,R365,R366.

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
When the example runs successfully, you can see the similar information from the terminal as below.

For flexspi_nor targets, the result is:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~
FLEXSPI example started!
Vendor ID: 0x16
Erasing Serial NOR over FlexSPI...
Erase data - successfully.
Program data - successfully.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~


For ram/sdram targets, the result is:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~
FLEXSPI example started!
Vendor ID: 0x16
Erasing whole chip over FlexSPI...
Erase finished !
Erasing Serial NOR over FlexSPI...
Erase data - successfully.
Program data - successfully.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Toolchain supported
===================
- IAR embedded Workbench  8.32.1
- Keil MDK  5.26
- GCC ARM Embedded  7.3.1
- MCUXpresso 10.3.0

