Overview
========
The flexio_spi_master_edma_lpspi_slave example shows how to use flexio spi master driver in edma way:

In this example, a flexio simulated master connect to a lpspi slave .



Toolchain supported
===================
- IAR embedded Workbench  9.32.1
- Keil MDK  5.37
- MCUXpresso  11.7.0
- GCC ARM Embedded  10.3.1

Board settings
==============
Remove 0Ω resistor to R200,R406,R408,R404.

To make the example work, connections needed to be as follows:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        SLAVE           connect to      MASTER
Pin Name   Board Location     Pin Name    Board Location
SIN        J10-10               SOUT      J26-6
SOUT       J10-8                SIN       J26-4
SCK        J10-12               SCK       J26-2
PCS0       J10-6                PCS0      J26-8
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
FLEXIO Master edma - LPSPI Slave interrupt example start.

This example use one flexio spi as master and one lpspi instance as slave on one board.

Master uses edma and slave uses interrupt way.

Please make sure you make the correct line connection. Basically, the connection is:

FLEXIO_SPI_master -- LPSPI_slave

      CLK        --    CLK

      PCS        --    PCS

      SOUT       --    SIN

      SIN        --    SOUT

This is LPSPI slave call back.

FLEXIO SPI master <-> LPSPI slave transfer all data matched!

End of example.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Note
This case use flexio to simulate spi protocol, 

Owing to this simulate process is software and executed in qspi_flash for flexspi_nor target, 

but external flash's speed can not support to the rate of SPI transmit because of effected by performance. 

So the settings for baudrate of transmission do not more than 150k
