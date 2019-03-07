Overview
========
The mmdc example shows how to use DDR controller driver to initialize the external DDR chip.
After the DDR initialization, DDR address access is very simple.




Toolchain supported
===================
- IAR embedded Workbench 7.80.4
- GCC ARM Embedded 2016-5.4-q3

Hardware requirements
=====================
- Micro USB cable
- MCIMX6UL-EVK 
- Personal Computer

Board settings
==============
Note: 
This demo don't support run by using MFGTool.

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

 MMDC initialized(MMDC already initialized).

 DDR work freq 396000000 HZ

 DDR Write finished!

 DDR Read Start, Start Address 0x80000000, Data Length 512 !

 DDR Read finished!

 DDR Write Data and Read Data Compare Start!

 DDR Write Data and Read Data Succeed.

 DDR Example End.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

