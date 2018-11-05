Overview
========

The FLEXBUS (External Bus Interface) example show how to write/read to external memories (MRAM)
by using FLEXBUS driver.

In this example, FLEXBUS driver is configured with  some specific parameter:
    - 1 byte port size of transfer
    - Wait 2 states
    - MRAM address for using FlexBus
    - 512 Kbytes memory size

Toolchain supported
===================
- Keil MDK 5.24a
- IAR embedded Workbench 8.22.2
- GCC ARM Embedded 7-2017-q4-major
- MCUXpresso10.2.0

Hardware requirements
=====================
- Mini/micro USB cable
- TWR-KV58F220M board
- Personal Computer
- TWR-MEM board
- TWR-ELEV primary

Board settings
==============
No special settings are required.

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the example runs successfully, you can see the similar information from the terminal as below.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
FLEXBUS Example.

Initialize FLEXBUS.

Start write/read MRAM.

FLEXBUS (MRAM) test: succeed.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

