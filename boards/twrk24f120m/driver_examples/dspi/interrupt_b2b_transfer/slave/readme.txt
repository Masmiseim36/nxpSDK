Overview
========
The dspi_interrupt_b2b_transfer example shows how to use DSPI driver in interrupt way:

In this example , we need two boards, one board used as DSPI master and another board used as DSPI slave.
The file 'dspi_interrupt_b2b_transfer_slave.c' includes the DSPI slave code.
This example uses the transactional API in DSPI driver.

1. DSPI master send/received data to/from DSPI slave in interrupt . (DSPI Slave using interrupt to receive/send the data)

Toolchain supported
===================
- IAR embedded Workbench 8.11.3
- Keil MDK 5.23
- GCC ARM Embedded 6-2017-q2
- Kinetis Development Studio IDE 3.2.0
- MCUXpresso10.1.0

Hardware requirements
=====================
- Mini USB cable
- Two TWR-K24F120M boards
- Personal Computer
- Two Elevator Tower boards

Board settings
==============
SPI board to board:
Transfers data through SPI interface. SPI0 pins of the master board are
connected to the SPI1 pins of the slave board.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MASTER BOARD           CONNECTS TO         SLAVE BOARD
Pin Name   Board Location     Pin Name  Board Location
SOUT       B45                SIN       B11
SIN        B44                SOUT      B10
SCK        B64                SCK       B07
PCS0       B63                PCS0      B09
GND        B2                 GND       B2
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
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the example runs successfully, the following message is displayed in the terminal:
~~~~~~~~~~~~~~~~~~~~~~~~
DSPI example start.
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

