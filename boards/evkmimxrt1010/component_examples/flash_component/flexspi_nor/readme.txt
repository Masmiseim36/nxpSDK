Overview
========
The nor flash demo application demonstrates the use of nor flash component to erase, program, and read an
external flash device.

Toolchain supported
===================
- GCC ARM Embedded  10.3.1
- IAR embedded Workbench  9.32.1
- Keil MDK  5.37
- MCUXpresso  11.7.0

Hardware requirements
=====================
- Mini/micro USB cable
- EVK-MIMXRT1010 board
- Personal Computer

Board settings
==============

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
The log below shows the output of the flash component demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
***NOR Flash Component Demo Start!***


***NOR Flash Page 0 Read/Write Success!***


***NOR Flash Page 1 Read/Write Success!***


***NOR Flash Page 2 Read/Write Success!***


.....


***NOR Flash Page 13 Read/Write Success!***


***NOR Flash Page 14 Read/Write Success!***


***NOR Flash Page 15 Read/Write Success!***

***NOR Flash All Pages Read/Write Success!***

~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Known issue
The MCUX release target and armgcc flexspi_nor_release run fail.
