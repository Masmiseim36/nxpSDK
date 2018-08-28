Overview
========
The flexio_spi_master_interrupt_dspi_slave example shows how to use flexio spi master driver in interrupt way:

In this example, a flexio simulated master connect to a dspi slave .

Toolchain supported
===================
- Keil MDK 5.24a
- IAR embedded Workbench 8.22.2
- GCC ARM Embedded 7-2017-q4-major
- MCUXpresso10.2.0

Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-K28F boards
- Personal Computer

Board settings
==============
The flexio_spi_master_interrupt_dspi_slave example is requires connecting between FlexIO pins with DSPI pins
The connection should be set as following:
- J27-19(PTD8),  J27-9(PTA17) connected
- J4-12(PTB2),   J27-12(PTA14) connected
- J27-20(PTD9),  J27-10(PTA16) connected
- J4-10(PTB3),   J27-11(PTA15) connected

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
FLEXIO Master - DSPI Slave interrupt example start.

This example use one flexio spi as master and one dspi instance as slave on one board.

Master and slave are both use interrupt way.

Please make sure you make the correct line connection. Basically, the connection is:

FLEXI_SPI_master -- DSPI_slave

   CLK      --    CLK

   PCS      --    PCS

   SOUT     --    SIN

   SIN      --    SOUT

This is DSPI slave call back.

FLEXIO SPI master <-> DSPI slave transfer all data matched!
~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

