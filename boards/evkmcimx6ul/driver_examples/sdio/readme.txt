Overview
========
The SDIO project is a demonstration program that uses the SDK software. It reads/writes the SDIO card reigister. The purpose of this example is to show how to use SDio driver and this is a very simple example.

Toolchain supported
===================
- IAR embedded Workbench 7.80.4
- GCC ARM Embedded 2016-5.4-q3

Hardware requirements
=====================
- Micro USB cable
- Jlink
- MCIMX6UL-EVK 
- Personal Computer

Board settings
==============
Please insert the SDIO card into card slot(P1701)

Prepare the Demo
================
1.  Connect 5V power supply and JLink Plus to the board, switch SW2001 to power on the board.
2.  Connect a USB cable between the host PC and the J1901 USB port on the target board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Reset the SoC and run the project.

Running the demo
================
When the demo runs successfully, the log would be seen on the terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

SDIO card simple example.

SDIO init Done

Read function CIS, in direct way

Read function CIS, in extended way, non-block mode, non-word aligned size

Read function CIS, in extended way, block mode, non-word aligned size
The read content is consistent.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

