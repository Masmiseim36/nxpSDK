Overview
========
The flexio_spi_slave_interrupt_lpspi_master example shows how to use flexio spi slave driver in interrupt way:

In this example, a flexio simulated slave connect to a lpspi master.


Running the demo
================
You can see the similar message shows following in the terminal if the example runs successfully.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~
LPSPI Master interrupt - FLEXIO SPI Slave interrupt example start.

This example use one lpspi instance as master and one flexio spi slave on one board.

Master and slave are both use interrupt way.

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
Hardware requirements
=====================
- Mini/micro USB cable
- EVKB-IMXRT1050 board
- Personal Computer

Board settings
==============
Remove the resistor R334 and weld 0Ω resistor to R278,R279,R280,R281.

To make the example work, connections needed to be as follows:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
       MASTER           connect to      SLAVE
Pin Name   Board Location     Pin Name    Board Location
SOUT       J24-4                SIN       SW5-3
SIN        J24-5                SOUT      SW5-2
SCK        J24-6                SCK       SW5-1
PCS0       J24-3                PCS0      SW5-4
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


Toolchain supported
===================
- IAR embedded Workbench  8.32.1
- Keil MDK  5.26
- GCC ARM Embedded  7.3.1
- MCUXpresso 10.3.0

