Overview
========
The flexio_spi_slave_edma_dspi_master example shows how to use flexio spi slave driver in dma way:

In this example, a flexio simulated slave connect to a dspi master.

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
The flexio_spi_slave_edma_dspi_master example is configured to use UART1 with PTE0 and PTE1 pins. To make JLink serial
device work, the jumpers on MAPS-DOCK board should be set as following:
- JP7 on MAPS-Dock: UART0 related jumpers connected

The flexio_spi_slave_edma_dspi_master example is configured to use FLEXIO D0, D1, D2 and D3 with PTC12, PTC13, PTC14
and PTC15 to simulate spi slave and communicate with a on board dspi master example, with pin connections below:

PTC12: FLEXIO_SPI_SOUT_PIN, CN9D-84(slave) <--> CN9D-100(master)
PTC13: FLEXIO_SPI_SIN_PIN,  CN9D-85(slave) <--> CN9D-99(master)
PTC14: FLEXIO_SPI_CLK_PIN,  CN9D-86(slave) <--> CN9D-98(master)
PTC15: FLEXIO_SPI_PCS_PIN,  CN9D-87(slave) <--> CN9D-97(master)

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

