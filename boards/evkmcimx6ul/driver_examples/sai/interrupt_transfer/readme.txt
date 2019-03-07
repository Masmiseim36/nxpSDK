Overview
========
The sai_interrupt_transfer example shows how to use sai driver with interrupt:

In this example, one sai instance playbacks the audio data stored in flash/SRAM using interrupt.

Toolchain supported
===================
- IAR embedded Workbench 7.80.4
- GCC ARM Embedded 2016-5.4-q3

Hardware requirements
=====================
- Micro USB cable
- MCIMX6UL-EVK  board
- 5V power supply
- Personal Computer
- Headphone

Board settings
==============
No special is needed.

Prepare the Demo
================
1.  Connect 5V power supply to the board.
2.  Connect two USB cables from the host PC to the J1901 and the J1102 USB port on the target board.
3.  Connect Headphone to the J1401 port on the target board
4.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
5.  Use MFGTool to download the program to the target board and run it. SAI module reused several pins 
    of JTAG, so it could not be downloaded and run by using IAR IDE. 

Running the demo
================
When the demo runs successfully, you can hear the tone and the log would be seen on the OpenSDA terminal like:

~~~~~~~~~~~~~~~~~~~
SAI example started!
SAI example finished!
 ~~~~~~~~~~~~~~~~~~~
Customization options
=====================

