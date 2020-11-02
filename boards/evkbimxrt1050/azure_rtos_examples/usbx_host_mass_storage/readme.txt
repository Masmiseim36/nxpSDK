Overview
========
This example illustrates USBX Host Mass Storage.


Toolchain supported
===================
- IAR embedded Workbench  8.50.1
- GCC ARM Embedded  9.2.1
- MCUXpresso  11.2.0

Hardware requirements
=====================
- Mini/micro USB cable
- USB A to micro USB cable
- Target board
- Personal Computer
- USB disk

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
3.  Compile the demo with the configuration, "flexspi_nor_debug".
4.  Write the program to the flash of the target board.
5.  Press the reset button on your board to start the demo.
6.  Connect a USB disk to the board.
7.  The serial terminal will dump the files name stored in the disk.
Running the demo
================
After writing the program to the flash of the target board,
press the reset button on your board to start the demo.
The serial port will output:

USBX device mass storage example

Then, connect a U-disk to the USB device port of the board.
The example will scan the disk and dump the file name to
the serial port and read the file contents.

For example:

Find File: 1234.text
Find File: test.text
Customization options
=====================

