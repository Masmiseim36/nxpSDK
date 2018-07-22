Overview
========
The freertos_dspi example shows how to use DSPI driver in FreeRTOS:

In this example , one dspi instance used as DSPI master with blocking and another dspi instance used as DSPI slave .

1. DSPI master sends/receives data using task blocking calls to/from DSPI slave. (DSPI Slave uses interrupt to receive/
send the data)


Toolchain supported
===================
- IAR embedded Workbench 7.50.1
- Keil MDK 5.17
- GCC ARM Embedded 2015-4.9-q3
- Kinetis Development Studio IDE 3.0.0
- Atollic TrueSTUDIO 5.4.0

Hardware requirements
=====================
- Mini USB cable
- FRDM-K82F board
- Personal Computer

Board settings
==============
SPI one board:
Transfer data from instance0 to instance 1 of SPI interface.
SPI0 pins are connected with SPI1 pins of board
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
INSTANCE0(SPI0)     CONNECTS TO         INSTANCE1(SPI1)
Pin Name   Board Location     Pin Name  Board Location
MOSI       J1 pin 7           MISO      J22 pin 6
MISO       J1 pin 13          MOSI      J22 pin 7
SCK        J1 pin 15          SCK       J22 pin 5
PCS0       J1 pin 9           PCS0      J2 pin 6
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
When the example runs successfully, you can see the similar information on the terminal as below.

DSPI interrupt example start.
This example use one dspi instance as master and another as slave on one board.
Please make sure you make the correct line connection. Basically, the connection is:
DSPI_master -- DSPI_slave
   CLK      --    CLK
   PCS      --    PCS
   SOUT     --    SIN
   SIN      --    SOUT
DSPI master transfer completed successfully.

DSPI slave transfer completed successfully.

DSPI transfer all data matched!

Customization options
=====================


