Overview
========
The flexio_spi_slave_interrupt_spi_master example shows how to use flexio spi slave driver in interrupt way:

In this example, a flexio simulated slave connect to a spi master.

Toolchain supported
===================
- IAR embedded Workbench  8.32.3
- Keil MDK  5.27
- GCC ARM Embedded  8.2.1
- MCUXpresso  11.0.1

Hardware requirements
=====================
- Mini USB cable
- FRDM-K32L2B board
- Personal Computer

Board settings
==============
The flexio_spi_slave_interrupt_spi_master example is requires connecting between FlexIO pins with SPI pins
The connection should be set as following:
- J1-7, J2-6 connected
- J1-9, J2-8 connected
- J1-11, J2-10 connected
- J1-15, J2-12 connected

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
SPI Master interrupt - FLEXIO SPI Slave interrupt example start.

This example use one spi instance as master and one flexio spi slave on one board.

Master and slave are both use interrupt way.

Please make sure you make the correct line connection. Basically, the connection is:

SPI_master -- FLEXIO_SPI_slave

   SCK      --    SCK

   PCS0     --    PCS0

   MOSI     --    MOSI

   MISO     --    MISO

This is FLEXIO SPI slave call back.

SPI master <-> FLEXIO SPI slave transfer all data matched!
~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

