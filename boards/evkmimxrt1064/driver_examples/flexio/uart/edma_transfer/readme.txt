Overview
========
The flexio_uart_edma example shows how to use flexio uart driver in edma way:

In this example, a flexio simulated uart connect to PC through USB-Serial, the board will send back all characters
that PC send to the board.

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
- USB to Com Converter

Board settings
==============
Connect pin:
- RX of USB2COM to SW5-1
- TX of USB2COM to SW5-2
- GND of USB2COM to J24-7

Prepare the Demo
================
1.  Connect the USB2COM Converter to the host PC
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
When the demo runs successfully, the log would be seen on the UART Terminal port which connected to the USB2COM like:

~~~~~~~~~~~~~~~~~~~~~
Flexio uart edma example
Board receives 8 characters then sends them out
Now please input:
~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

