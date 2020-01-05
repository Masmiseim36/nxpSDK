Overview
========
The spi_polling_board2board_master example shows how to use spi driver as master to do board to board transfer with 
polling:

In this example, one spi instance as master and another spi instance on othereboard as slave. Master sends a piece of
data to slave, and receive a piece of data from slave. This example checks if the data received from slave is correct.

Toolchain supported
===================
- IAR embedded Workbench 8.11.3
- Keil MDK 5.23
- GCC ARM Embedded 6-2017-q2
- Kinetis Development Studio IDE 3.2.0
- MCUXpresso10.1.0

Hardware requirements
=====================
- Mini USB cable
- FRDM-KL03Z board
- Personal Computer

Board settings
==============
SPI one board:
  + Transfer data from MASTER_BOARD to SLAVE_BOARD of SPI interface, SPI0 pins of MASTER_BOARD are connected with
    SPI1 pins of SLAVE_BOARD
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
           MASTER_BOARD    CONNECTS TO   SLAVE_BOARD
Pin Name   Board Location                Board Location
MOSI       J2-4                          J2-4
MISO       J2-5                          J2-5
SS         J2-3                          J2-3
CLK        J2-6                          J2-6
GND        J2-7                          J2-7
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Prepare the Demo
================
1.  Connect a mini USB cable between the PC host and the OpenSDA USB port on the board.
2.  Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 9600 baud rate
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
Master Start...

Succeed!
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

