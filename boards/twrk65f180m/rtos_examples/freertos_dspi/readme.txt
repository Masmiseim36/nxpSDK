Overview
========
The freertos_dspi example shows how to use DSPI driver in FreeRTOS:

In this example , one dspi instance used as DSPI master with blocking and another dspi instance used as DSPI slave .

1. DSPI master sends/receives data using task blocking calls to/from DSPI slave. (DSPI Slave uses interrupt to receive/
send the data)


Toolchain supported
===================
- IAR embedded Workbench 7.70.1
- Keil MDK 5.20
- GCC ARM Embedded 2015-4.9-q3
- Kinetis Development Studio IDE 3.2.0
- Atollic TrueSTUDIO 5.5.2

Hardware requirements
=====================
- Mini USB cable
- TWR-K65F180M board
- Personal Computer
- Elevator Tower

Board settings
==============
SPI one board:
Transfer data from instance2 to instance 1 of SPI interface.
SPI2 pins are connected with SPI1 pins of board
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
INSTANCE0(SPI2)     CONNECTS TO         INSTANCE1(SPI1)
Pin Name   Board Location     Pin Name  Board Location
MOSI       B44                MISO      B11
MISO       B45                MOSI      B10
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

