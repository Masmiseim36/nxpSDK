Overview
========
The dspi_polling_b2b_transfer example shows how to use DSPI driver in polling way:

In this example , we need two boards, one board used as DSPI master and another board used as DSPI slave.
The file 'dspi_polling_b2b_transfer_master.c' includes the DSPI master code.

1. DSPI master send/received data to/from DSPI slave in polling . (DSPI Slave using interrupt to receive/send the data)

Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- Keil MDK  5.29
- GCC ARM Embedded  8.3.1
- MCUXpresso  11.1.0

Hardware requirements
=====================
- Mini/micro USB cable
- Two board TWR-K80F150M
- Personal Computer
- Primary Elevator Tower

Board settings
==============
DSPI board to board:
Transfers data through the instance 0 of the SPI interface. SPI0 pins of the master board are
connected to the SPI0 pins of the slave board.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MASTER BOARD           CONNECTS TO         SLAVE BOARD
Pin Name   Primary Elevator               Pin Name  Primary Elevator
MISO       B44                            MISO      B45
MOSI       B45                            MOSI      B44
SCK        B48                            SCK       B48
PCS0       B46                            PCS0      B46
GND        B2                             GND       B2
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
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DSPI board to board polling example.
This example use one board as master and another as slave.
Master uses polling way and slave uses interrupt way.  Slave should start first.
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

 Master received:

 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 10
 11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F 20
 21 22 23 24 25 26 27 28 29 2A 2B 2C 2D 2E 2F 30
 31 32 33 34 35 36 37 38 39 3A 3B 3C 3D 3E 3F 00
DSPI transfer all data matched!

 Press any key to run again
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Slave:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 DSPI board to board polling example.

 Slave example is running...
This is DSPI slave transfer completed callback.
It's a successful transfer.

 Slave received:
     01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 10
     11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F 20
     21 22 23 24 25 26 27 28 29 2A 2B 2C 2D 2E 2F 30
     31 32 33 34 35 36 37 38 39 3A 3B 3C 3D 3E 3F 00
This is DSPI slave transfer completed callback.
It's a successful transfer.

 Slave example is running...

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

