Overview
========
The spi_interrupt_b2b_master example shows how to use spi functional API to do interrupt transfer as a master:

In this example, the spi instance as master. Master sends a piece of data to slave, receive data from slave,and check
if the data master received is correct. This example needs to work with spi_interrupt_b2b_slave example.

Toolchain supported
===================
- Keil MDK 5.24a
- IAR embedded Workbench 8.22.2
- GCC ARM Embedded 7-2017-q4-major
- MCUXpresso10.2.0

Hardware requirements
=====================
- Mini USB cable
- Two FRDM-KL46Z boards
- Personal Computer

Board settings
==============
SPI board to board:
Transfers data through SPI interface. SPI0 pins of the master board are
connected to the SPI1 pins of the slave board.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MASTER BOARD           CONNECTS TO         SLAVE BOARD
Pin Name   Board Location     Pin Name  Board Location
MISO       J2 pin 15          MISO      J2 pin 10
MOSI       J2 pin 13          MOSI      J2 pin 8
SCK        J2 pin 11          SCK       J2 pin 12
PCS0       J2 pin 9           PCS0      J2 pin 6
GND        J2 pin 14          GND       J2 pin 14
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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
When the example runs successfully, the following message is displayed in the terminal:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SPI board to board interrupt master example started!

SPI transfer finished!
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

