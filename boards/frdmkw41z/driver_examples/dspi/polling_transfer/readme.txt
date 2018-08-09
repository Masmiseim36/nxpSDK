Overview
========
The dspi_polling_transfer example shows how to use DSPI driver in polling way:

In this example , one dspi instance used as DSPI master and another dspi instance used as DSPI slave in the same board.

1. DSPI master send/received data to/from DSPI slave in polling . (DSPI Slave using interrupt to receive/send the data)

Toolchain supported
===================
- IAR embedded Workbench 7.80.4
- Keil MDK 5.21a
- GCC ARM Embedded 2016-5.4-q3
- Kinetis Development Studio IDE 3.2.0
- MCUXpresso0.8

Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-KW41Z board
- Personal Computer

Board settings
==============
SPI one board:
Transfer data from instance0 to instance 1 of SPI interface.
SPI0 pins are connected with SPI1 pins of board
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
INSTANCE0(SPI0)     CONNECTS TO         INSTANCE1(SPI1)
Pin Name   Board Location     Pin Name  Board Location
MOSI       J1  pin 6          MISO      J2  pin 5
MISO       J1  pin 7          MOSI      J2  pin 4
SCK        J1  pin 4          SCK       J2  pin 6
PCS0       J1  pin 3          PCS0      J2  pin 3
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
The connection should be set as following:
- J28-1 , J28-2 connected
- J29-1 , J29-2 connected

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
DSPI polling example start.
This example use one dspi instance as master and another as slave on one board.
Master uses polling way and slave uses interrupt way.
Please make sure you make the correct line connection. Basically, the connection is:

DSPI_master -- DSPI_slave

   CLK      --    CLK
   PCS      --    PCS
   SOUT     --    SIN
   SIN      --    SOUT
This is DSPI slave transfer completed callback.
It's a successful transfer.

DSPI transfer all data matched!
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

