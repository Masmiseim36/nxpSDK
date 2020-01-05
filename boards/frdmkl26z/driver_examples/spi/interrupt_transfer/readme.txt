Overview
========
The spi_interrupt example shows how to use spi driver with interrupt:

In this example, one spi instance as master and another spi instance as slave. Master sends a piece of data to slave,
and check if the data slave received is correct.

Toolchain supported
===================
- IAR embedded Workbench 7.80.4
- Keil MDK 5.21a
- GCC ARM Embedded 2016-5.4-q3
- Kinetis Development Studio IDE 3.2.0
- MCUXpresso0.8

Hardware requirements
=====================
- Mini USB cable
- FRDM-KL26Z board
- Personal Computer

Board settings
==============
SPI one board:
  + Transfer data from instance0 to instance 1 of SPI interface, SPI0 pins are connected with
    SPI1 pins of board
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
INSTANCE0        CONNECTS TO         INSTANCE1
Pin Name   Board Location     Pin Name  Board Location
MOSI       J1 pin 9           MOSI      J2 pin 8
MISO       J1 pin 11          MISO      J2 pin 10
SCK        J4 pin 9           SCK       J2 pin 12
PCS0       J1 pin 7           PCS0      J2 pin 6
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
SPI one board interrupt example started!

SPI transfer finished!
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

