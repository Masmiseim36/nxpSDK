Overview
========
CMSIS-Driver defines generic peripheral driver interfaces for middleware making it reusable across a wide 
range of supported microcontroller devices. The API connects microcontroller peripherals with middleware 
that implements for example communication stacks, file systems, or graphic user interfaces. 
More information and usage methord please refer to http://www.keil.com/pack/doc/cmsis/Driver/html/index.html.

The cmsis_dspi_edma_transfer example shows how to use CMSIS DSPI driver in edma way:
In this example , one dspi instance used as master and another dspi instance used as slave in the same board.
DSPI master send/received data to/from DSPI slave in edma . (DSPI Slave using edma to receive/send the data)


Toolchain supported
===================
- IAR embedded Workbench 8.11.3
- Keil MDK 5.23
- GCC ARM Embedded 6-2017-q2
- Kinetis Development Studio IDE 3.2.0
- MCUXpresso10.1.0

Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-K28F board
- Personal Computer

Board settings
==============
SPI one board:
Transfer data from instance0 to instance 1 of SPI interface.
SPI0 pins are connected with SPI2 pins of board
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
INSTANCE0(SPI0)     CONNECTS TO         INSTANCE1(SPI2)
Pin Name   Board Location     Pin Name  Board Location
MISO(PTA17)       J27 pin 20          MISO(PTD13)      J27 pin 26
MOSI(PTA16)       J27 pin 19          MOSI(PTD14)      J27 pin 27
SCK(PTA15)        J27 pin 18          SCK(PTD12)       J27 pin 25
PCS0(PTA14)       J27 pin 17          PCS0(PTD11)      J27 pin 24
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

