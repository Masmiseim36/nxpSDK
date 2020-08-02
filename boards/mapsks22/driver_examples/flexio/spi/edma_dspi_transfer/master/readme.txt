Overview
========
The flexio_spi_master_edma_dspi_slave example shows how to use flexio spi master driver in edma way:

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
- MAPS-KS22F256 board & MAPS-DOCK board
- Personal Computer

Board settings
==============
The flexio_spi_master_edma_dspi_slave example is configured to use UART1 with PTE0 and PTE1 pins. To make JLink serial
device work, the jumpers on MAPS-DOCK board should be set as following:
- JP7 on MAPS-Dock: UART0 related jumpers connected

The flexio_spi_master_edma_dspi_slave example is configured to use FLEXIO D0, D1, D2 and D3 with PTC12, PTC13, PTC14
and PTC15 to simulate spi master and communicate with a on board dspi slave, with pin connections below:

PTC12: FLEXIO_SPI_SOUT_PIN, CN9D-84(master) <--> CN9D-100(slave)
PTC13: FLEXIO_SPI_SIN_PIN,  CN9D-85(master) <--> CN9D-99(slave)
PTC14: FLEXIO_SPI_CLK_PIN,  CN9D-86(master) <--> CN9D-98(slave)
PTC15: FLEXIO_SPI_PCS_PIN,  CN9D-87(master) <--> CN9D-97(slave)

Prepare the Demo
================
1. Connect a mini USB cable between the PC host and the USB port (CN14) on MAPS-DOCK board.
2. Open a serial terminal on PC for JLink serial device with these settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running
   the demo.

Running the demo
================
When the demo runs successfully, the log would be seen on the OpenSDA terminal like:

~~~~~~~~~~~~~~~~~~~~~
FLEXIO Master edma - DSPI Slave interrupt example start.

This example use one flexio spi as master and one dspi instance as slave on one board.

Master uses edma and slave uses interrupt way.

Please make sure you make the correct line connection. Basically, the connection is:

FLEXIO_SPI_master -- DSPI_slave

   CLK      --    CLK

   PCS      --    PCS

   SOUT     --    SIN

   SIN      --    SOUT

This is DSPI slave call back.

FLEXIO SPI master <-> DSPI slave transfer all data matched!
~~~~~~~~~~~~~~~~~~~~~

The MOSI pin should connect to MOSI pin on the slave board
Customization options
=====================

