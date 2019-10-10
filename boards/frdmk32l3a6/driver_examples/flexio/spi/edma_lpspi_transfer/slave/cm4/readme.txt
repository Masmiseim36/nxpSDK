Overview
========
The flexio_spi_slave_edma_lpspi_master example shows how to use flexio spi slave driver in edma way:

In this example, a flexio simulated slave connect to a lpspi master.



Toolchain supported
===================
- IAR embedded Workbench  8.32.3
- Keil MDK  5.27
- MCUXpresso  11.0.1
- GCC ARM Embedded  8.2.1

Hardware requirements
=====================
- Mini USB cable
- FRDM-K32L3A6 board
- Personal Computer

Board settings
==============
To make the example work, connections needed to be as follows:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
       SLAVE           connect to      MASTER
Pin Name       Board Location         Pin  Name  Board Location
SOUT(PTE19)       J1 pin 7            SIN (PTC11)    J4 pin 2
SIN (PTE13)       J1 pin 1            SOUT(PTC9)     J2 pin 18
SCK (PTB15)       J3 pin 15           SCK (PTC30)    J2 pin 13
PCS0(PTE21)      J1 pin 11            PCS0(PTC12)    J4 pin 4
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Prepare the Demo
================
1. Connect a mini USB cable between the PC host and the OpenSDA USB port on the board.
2. Open a serial terminal on PC for OpenSDA serial device with these settings:
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
You can see the similar message shows following in the terminal if the example runs successfully.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~
LPSPI Master interrupt - FLEXIO SPI Slave edma example start.

This example use one lpspi instance as master and one flexio spi slave on one board.

Master uses interrupt and slave uses edma way.

Please make sure you make the correct line connection. Basically, the connection is:

LPSPI_master -- FLEXIO_SPI_slave

   CLK      --    CLK

   PCS      --    PCS

   SOUT     --    SIN

   SIN      --    SOUT

This is FLEXIO SPI slave call back.

LPSPI master <-> FLEXIO SPI slave transfer all data matched!

End of Example.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

