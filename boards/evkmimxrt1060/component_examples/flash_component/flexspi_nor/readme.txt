Overview
========
The nor flash demo application demonstrates the use of nor flash component to erase, program, and read an
external flash device.

Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- Keil MDK  5.29
- GCC ARM Embedded  8.3.1
- MCUXpresso  11.1.0

Hardware requirements
=====================
- Mini/micro USB cable
- EVK-MIMXRT1060 board
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
The log below shows the output of the hello world demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
***NOR Flash Component Demo Start!***

***NOR Flash Initialization Start!***

***NOR Flash Initialization Success!***

***NOR Flash Erase Chip Start!***


***NOR Flash Page 0 Read/Write Success!***


***NOR Flash Page 1 Read/Write Success!***
.....
.....
***NOR Flash Page 254 Read/Write Success!***


***NOR Flash Page 255 Read/Write Success!***

***NOR Flash All Pages Read/Write Success!***
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

