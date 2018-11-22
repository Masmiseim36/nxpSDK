Overview
========
The flexspi_hyper_flash_polling_transfer example shows how to use flexspi driver with polling:

In this example, flexspi will send data and operate the external Hyper flash connected with FLEXSPI. Some simple flash command will
be executed, such as Read ID, Erase Sector and Program Buffer.
Example will first configures hyper flash to enter ASO mode, read ID-CFI parameters, then exit ASO mode.
Second, the example erase a sector in flash, check if the erase is successful and program the same sector, then read back
the sector data. At last check if the data read back is correct.

Toolchain supported
===================
- Keil MDK 5.25
- IAR embedded Workbench 8.30.1
- GCC ARM Embedded 7-2017-q4-major
- MCUXpresso10.2.1

Hardware requirements
=====================
- Mini/micro USB cable
- EVK-MIMXRT1064 board
- Personal Computer

Board settings
==============
The board enable QSPI flash (U33) by default. To enables hyper flash (U19), some hardware rework is needed:
Mount R356,R361,R362,R363,R364,R365,R366 and DNP R153,R154,R155,R156,R157,R158.

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

~~~~~~~~~~~~~~~~~~~~~~~~~~~~
FLEXSPI hyperflash example started!
Entering the ASO mode
Found the HyperFlash by CFI
Erasing Serial NOR over FlexSPI...
Erase data - successfully.
Program data - successfully.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

