Overview
========
The lpuart_polling Example project is to demonstrate usage of the KSDK lpuart driver.
In the example, you can send characters to the console back and they will be printed out onto console
 instantly.

Toolchain supported
===================
- Keil MDK 5.24a
- IAR embedded Workbench 8.22.2
- GCC ARM Embedded 7-2017-q4-major
- MCUXpresso10.2.0

Hardware requirements
=====================
- Mini/Micro USB cable
- FRDM-K22F board
- Personal Computer
- USB to Com Converter

Board settings
==============
Connect pin:
- RX of USB2COM to J1-4
- TX of USB2COM to J1-2
- GND of USB2COM to J2-14

Prepare the Demo
================
1.  Connect USB to Com Converter between the PC host and the port on the board.
2.  Open a serial terminal on PC for serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Reset the SoC and run the project.

Running the demo
================
When the demo runs successfully, the log would be seen on the LPUART Terminal port which connected to the USB2COM like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Lpuart polling example
Board will send back received characters
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

