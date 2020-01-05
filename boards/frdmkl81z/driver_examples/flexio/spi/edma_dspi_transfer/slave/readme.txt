Overview
========
The flexio_spi_slave_edma_dspi_master example shows how to use flexio spi slave driver in dma way:

In this example, a flexio simulated slave connect to a dspi master.
Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-KL81Z board
- Personal Computer

Board settings
==============
The example is requires connecting between FlexIO pins with DSPI pins
The connection should be set as following:
- J2-8, J2-20 connected
- J2-10, J2-18 connected
- J2-6, J4-12 connected
- J2-12, J4-10 connected

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

Toolchain supported
===================
- IAR embedded Workbench  8.32.1
- Keil MDK  5.26
- GCC ARM Embedded  7.3.1
- MCUXpresso 10.3.0

