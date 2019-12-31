Overview
========
The freertos_dspi example shows how to use DSPI driver in FreeRTOS:

In this example , one dspi instance used as DSPI master with blocking and another dspi instance used as DSPI slave .

1. DSPI master sends/receives data using task blocking calls to/from DSPI slave. (DSPI Slave uses interrupt to receive/
send the data)


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
- Two FRDM-KW36 boards
- Personal Computer

Board settings
==============
DSPI board to board:
Transfers data through the instance 1 of the SPI interface. SPI1 pins of the master board are
connected to the SPI1 pins of the slave board. Use SPI_MASTER_SLAVE macro definition to select
between master and slave mode.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MASTER BOARD        CONNECTS TO    SLAVE BOARD
Pin Name   Board pin               Pin Name  Board pin
SCK        J1  pin 7               SCK       J1  pin 7
SIN        J1  pin 5               SOUT      J2  pin 1
SOUT       J2  pin 1               SIN       J1  pin 5
PCS0       J2  pin 3               PCS0      J2  pin 3
GND        J3  pin 6               GND       J3  pin 6
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
The connection should be set as following:
- J9-1 , J9-2 connected
- J7-1 , J7-2 connected

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

