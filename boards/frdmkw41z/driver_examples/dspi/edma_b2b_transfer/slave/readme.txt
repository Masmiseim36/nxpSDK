Overview
========
The dspi_edma_b2b_transfer example shows how to use DSPI driver in edma way:

In this example , we need two boards, one board used as DSPI master and another board used as DSPI slave.
The file 'dspi_edma_b2b_transfer_slave.c' includes the DSPI slave code.

1. DSPI master send/received data to/from DSPI slave in edma . (DSPI Slave using edma to receive/send the data)

Toolchain supported
===================
- IAR embedded Workbench 7.80.4
- Keil MDK 5.21a
- GCC ARM Embedded 2016-5.4-q3
- Kinetis Development Studio IDE 3.2.0
- MCUXpresso0.8

Hardware requirements
=====================

Board settings
==============
DSPI board to board:
Transfers data through the instance 0 of the SPI interface. SPI0 pins of the master board are
connected to the SPI0 pins of the slave board.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MASTER BOARD        CONNECTS TO    SLAVE BOARD
Pin Name   Board pin               Pin Name  Board pin
MOSI       J1  pin 6               MISO      J1  pin 7
MISO       J1  pin 7               MOSI      J1  pin 6
SCK        J1  pin 4               SCK       J1  pin 4
PCS0       J1  pin 3               PCS0      J1  pin 3
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

Master:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DSPI board to board edma example.
This example use one board as master and another as slave.
Master and slave uses EDMA way.
Please make sure you make the correct line connection. Basically, the connection is:
DSPI_master -- DSPI_slave
   CLK      --    CLK
   PCS      --    PCS
   SOUT     --    SIN
   SIN      --    SOUT
   GND      --    GND

 Master transmit:

 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 10
 11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F 20
 21 22 23 24 25 26 27 28 29 2A 2B 2C 2D 2E 2F 30
 31 32 33 34 35 36 37 38 39 3A 3B 3C 3D 3E 3F 00
This is DSPI master edma transfer completed callback.

This is DSPI master edma transfer completed callback.

 Master received:

 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 10
 11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F 20
 21 22 23 24 25 26 27 28 29 2A 2B 2C 2D 2E 2F 30
 31 32 33 34 35 36 37 38 39 3A 3B 3C 3D 3E 3F 00
DSPI transfer all data matched!

 Press any key to run again
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Slave:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DSPI board to board edma example.

 Slave example is running...
This is DSPI slave edma transfer completed callback.


 Slave received:
     01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 10
     11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F 20
     21 22 23 24 25 26 27 28 29 2A 2B 2C 2D 2E 2F 30
     31 32 33 34 35 36 37 38 39 3A 3B 3C 3D 3E 3F 00
This is DSPI slave edma transfer completed callback.

 Slave example is running...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

