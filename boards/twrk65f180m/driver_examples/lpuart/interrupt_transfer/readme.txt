Overview
========
The lpuart_interrupt Example project is to demonstrate usage of the KSDK lpuart driver.
In the example, you can send characters to the console back and they will be printed out onto console
 in a group of 8 characters.

Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- Keil MDK  5.29
- GCC ARM Embedded  8.3.1
- MCUXpresso  11.1.0

Hardware requirements
=====================
- Micro/Mini USB cable
- TWR-K65F180M board
- Personal Computer
- Elevator Tower
- USB to Com Converter

Board settings
==============
Connect pin:
- RX of USB2COM to A59(Elevator)
- TX of USB2COM to A58(Elevator)
- GND of USB2COM to B2(Elevator)

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

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Lpuart interrupt example
Board receives 8 characters then sends them out
Now please input:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


Customization options
=====================

