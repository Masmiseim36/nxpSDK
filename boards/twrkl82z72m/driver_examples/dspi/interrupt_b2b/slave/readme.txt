Overview
========
The dspi_interrupt_b2b example shows how to use DSPI driver in interrupt way:

In this example , we need two boards , one board used as DSPI master and another board used as DSPI slave.
The file 'dspi_interrupt_b2b_slave.c' includes the DSPI slave code.
This example does not use the transactional API in DSPI driver. It's a demonstration that how to use the interrupt in KSDK driver.

1. DSPI master send/received data to/from DSPI slave in interrupt . (DSPI Slave using interrupt to receive/send the data)
Hardware requirements
=====================
- Mini/micro USB cable
- Two TWR-KL82Z72M boards
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
SIN        B44                            SOUT      B45
SOUT       B45                            SIN       B44
SCK        B48                            SCK       B48
PCS0       B46                            PCS0      B46
GND        B2                             GND       B2
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
When the example runs successfully, the following message is displayed in the terminal:

Master:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
dspi_functional_interrupt_board2board_master start.
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
 31 32 33 34 35 36 37 38 39 3A 3B 3C 3D 3E 3F 40

 DSPI transfer all data matched!

 Master received:

 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 10
 11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F 20
 21 22 23 24 25 26 27 28 29 2A 2B 2C 2D 2E 2F 30
 31 32 33 34 35 36 37 38 39 3A 3B 3C 3D 3E 3F 40

 Press any key to run again
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Slave:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 DSPI board to board functional interrupt example.
 Slave example is running...

 Slave receive below data in 1st round transfer:
     01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 10
     11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F 20
     21 22 23 24 25 26 27 28 29 2A 2B 2C 2D 2E 2F 30
     31 32 33 34 35 36 37 38 39 3A 3B 3C 3D 3E 3F 40

 Slave example is running...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Toolchain supported
===================
- IAR embedded Workbench  8.32.1
- Keil MDK  5.26
- GCC ARM Embedded  7.3.1
- MCUXpresso 10.3.0

