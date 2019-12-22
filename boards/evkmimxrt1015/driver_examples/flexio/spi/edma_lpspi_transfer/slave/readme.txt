Overview
========
The flexio_spi_slave_edma_lpspi_master example shows how to use flexio spi slave driver in edma way:

In this example, a flexio simulated slave connect to a lpspi master.



Toolchain supported
===================
- GCC ARM Embedded  8.3.1
- IAR embedded Workbench  8.40.2
- Keil MDK  5.29
- MCUXpresso  11.1.0

Hardware requirements
=====================

Board settings
==============

To make the example work, connections needed to be as follows:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
       MASTER           connect to      SLAVE
Pin Name   Board Location     Pin Name    Board Location
SOUT       J19-4                SIN       J18-5
SIN        J19-5                SOUT      J18-6
SCK        J19-6                SCK       J18-3
PCS0       J19-3                PCS0      J18-1
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

Note:
To debug in qspiflash, following steps are needed:
1. Select the flash target and compile.
3. Set the SW8: 1 off 2 off 3 on 4 off, then power on the board and connect USB cable to J41.
4. Start debugging in IDE.
   - Keil: Click "Download (F8)" to program the image to qspiflash first then clicking "Start/Stop Debug Session (Ctrl+F5)" to start debugging.
Customization options
=====================

