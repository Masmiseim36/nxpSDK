Overview
========
The dspi_polling_b2b_transfer example shows how to use DSPI driver in polling way:

In this example , we need two boards, one board used as DSPI master and another board used as DSPI slave.
The file 'dspi_polling_b2b_transfer_master.c' includes the DSPI master code.

1. DSPI master send/received data to/from DSPI slave in polling . (DSPI Slave using interrupt to receive/send the data)

Toolchain supported
===================
- IAR embedded Workbench 8.11.1
- Keil MDK 5.23
- GCC ARM Embedded 2016-5.4-q3
- Kinetis Development Studio IDE 3.2.0
- MCUXpresso10.3.1

Hardware requirements
=====================
- Mini/micro USB cable
- Two boards FRDM-KW36
- Personal Computer

Board settings
==============
DSPI board to board:
Transfers data through the instance 1 of the SPI interface. SPI1 pins of the master board are
connected to the SPI1 pins of the slave board.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MASTER BOARD           CONNECTS TO         SLAVE BOARD
Pin Name   Board pin               Pin Name  Board pin
SCK        J1  pin 7               SCK       J1  pin 7
SIN        J1  pin 5               SOUT      J2  pin 1
SOUT       J2  pin 1               SIN       J1  pin 5
PCS0       J2  pin 3               PCS0      J2  pin 3
GND        J3  pin 6               GND       J3  pin 6
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

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DSPI board to board polling example.
This example use one board as master and another as slave.
Master uses polling way and slave uses interrupt way.
Please make sure you make the correct line connection. Basically, the connection is:
DSPI_master -- DSPI_slave
   CLK      --    CLK
   PCS      --    PCS
   SOUT     --    SIN
   SIN      --    SOUT
   GND      --    GND

 Master transmit:

  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F 10
 11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F 20
 21 22 23 24 25 26 27 28 29 2A 2B 2C 2D 2E 2F 30
 31 32 33 34 35 36 37 38 39 3A 3B 3C 3D 3E 3F 40

DSPI transfer all data matched!

 Master received:

  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F 10
 11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F 20
 21 22 23 24 25 26 27 28 29 2A 2B 2C 2D 2E 2F 30
 31 32 33 34 35 36 37 38 39 3A 3B 3C 3D 3E 3F 40

 Press any key to run again
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

