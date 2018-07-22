Overview
========
CMSIS-Driver defines generic peripheral driver interfaces for middleware making it reusable across a wide 
range of supported microcontroller devices. The API connects microcontroller peripherals with middleware 
that implements for example communication stacks, file systems, or graphic user interfaces. 
More information and usage methord please refer to http://www.keil.com/pack/doc/cmsis/Driver/html/index.html.

The cmsis_dspi_interrupt_transfer example shows how to use CMSIS DSPI driver in interrupt way:
 
In this example , one dspi instance used as master and another dspi instance used as slave in the same board.
1. DSPI master send/received data to/from DSPI slave in edma . (DSPI Slave using edma to receive/send the data)


Toolchain supported
===================
- Keil MDK 5.20

Hardware requirements
=====================
- Mini USB cable
- TWR-K65F180M board
- Personal Computer
- Elevator Tower

Board settings
==============
SPI one board:
Transfer data from instance0 to instance 1 of SPI interface.
SPI2 pins are connected with SPI1 pins of board
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
INSTANCE0(SPI2)     CONNECTS TO         INSTANCE1(SPI1)
Pin Name   Board Location     Pin Name  Board Location
SIN        B44                SOUT      B11
SOUT       B45                SIN       B10
SCK        B48                SCK       B7
PCS0       B46                PCS0      B9
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
When the example runs successfully, you can see the similar information from the terminal as below.

CMSIS DSPI interrupt transfer example start.
This example use one dspi instance as master and another as slave on one board.
Master and slave are both use interrupt way.
Please make sure you make the correct line connection. Basically, the connection is:
DSPI_master -- DSPI_slave
   CLK      --    CLK
   PCS      --    PCS
   SOUT     --    SIN
   SIN      --    SOUT
This is SlaveSignalEvent.

DSPI transfer all data matched!

Customization options
=====================

