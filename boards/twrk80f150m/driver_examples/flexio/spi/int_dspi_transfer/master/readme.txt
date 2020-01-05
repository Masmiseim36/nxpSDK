Overview
========
The flexio_spi_master_interrupt_dspi_slave example shows how to use flexio spi master driver in interrupt way:

In this example, a flexio simulated master connect to a dspi slave .

Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- Keil MDK  5.29
- GCC ARM Embedded  8.3.1
- MCUXpresso  11.1.0

Hardware requirements
=====================
- Mini/micro USB cable
- TWR-K80F150M boards
- Primary Elevator
- Personal Computer

Board settings
==============
The flexio_spi_master_interrupt_dspi_slave example is requires connecting between FlexIO pins with DSPI pins
Insert TWR-K80F150M board into Primary Elevator. The connection should be set as following:
- A38-Elevator, B44-Elevator connected
- A37-Elevator, B45-Elevator connected
- B69-Elevator, B46-Elevator connected
- B70-Elevator, B48-Elevator connected

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

