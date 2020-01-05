Overview
========
The flexio_spi_slave_edma_dspi_master example shows how to use flexio spi slave driver in dma way:

In this example, a flexio simulated slave connect to a dspi master.

Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- Keil MDK  5.29
- GCC ARM Embedded  8.3.1
- MCUXpresso  11.1.0

Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-K82F boards
- Personal Computer

Board settings
==============
The flexio_spi_slave_edma_dspi_master example is requires connecting between FlexIO pins with DSPI pins
The connection should be set as following:
- J1-7, J12-16 connected
- J1-9, J12-5 connected
- J1-13, J12-15 connected
- J1-15, J12-6 connected

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
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the demo runs successfully, the log would be seen on the OpenSDA terminal like:

~~~~~~~~~~~~~~~~~~~~~
DSPI Master interrupt - FLEXIO SPI Slave edma example start.

This example use one dspi instance as master and one flexio spi as slave on one board.

Master uses interrupt and slave uses edma way.

Please make sure you make the correct line connection. Basically, the connection is:

DSPI_master -- FLEXIO_SPI_slave

   CLK      --    CLK

   PCS      --    PCS

   SOUT     --    SIN

   SIN      --    SOUT

This is FLEXIO SPI slave call back.

DSPI master <-> FLEXIO SPI slave transfer all data matched!
~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

