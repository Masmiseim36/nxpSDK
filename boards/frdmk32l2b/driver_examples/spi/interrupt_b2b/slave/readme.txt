Overview
========
The spi_interrupt_b2b_slave example shows how to use spi functional API to do interrupt transfer as a slave:

In this example, the spi instance as slave. Slave receives data froma master and send a peiece of data to master,
and check if the data slave received is correct. This example needs to work with spi_interrupt_b2b_master example.

Toolchain supported
===================
- IAR embedded Workbench  8.32.3
- Keil MDK  5.27
- GCC ARM Embedded  8.2.1
- MCUXpresso  11.0.1

Hardware requirements
=====================
- Mini USB cable
- Two FRDM-K32L2B boards
- Personal Computer

Board settings
==============
SPI board to board:
Transfers data through SPI interface. SPI0 pins of the master board are
connected to the SPI1 pins of the slave board.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MASTER BOARD           CONNECTS TO         SLAVE BOARD
Pin Name   Board Location     Pin Name  Board Location
MISO       J1 pin 11          MISO      J2 pin 10
MOSI       J1 pin 9           MOSI      J2 pin 8
SCK        J1 pin 15          SCK       J2 pin 12
PCS0       J1 pin 7           PCS0      J2 pin 6
GND        J2 pin 14          GND       J2 pin 14
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Prepare the Demo
================
1.  Connect a mini USB cable between the PC host and the OpenSDA USB port on the board.
2.  Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Reset the SoC and run the project.

Running the demo
================
When the demo runs successfully, the log would be seen on the OpenSDA terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SPI board to board interrupt slave example started!

SPI transfer finished!
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

