Overview
========
The flexio_spi_master_interrupt_lpspi_slave example shows how to use flexio spi master driver in interrupt way:

In this example, a flexio simulated master connect to a lpspi slave .



Toolchain supported
===================
- IAR embedded Workbench  9.10.2
- Keil MDK  5.34
- GCC ARM Embedded  10.2.1
- MCUXpresso  11.4.0

Hardware requirements
=====================
- Micro USB cable
- FRDM-KE17Z board
- Personal Computer

Board settings
==============
The example requires connecting the FLEXIO pins with the LPSPI pins
The connection should be set as following:
       MASTER           connect to      SLAVE
Pin Name   Board Location     Pin Name  Board Location
SOUT       J2 pin 15            SIN       J2 pin 10
SIN        J1 pin 7             SOUT      J2 pin 8
SCK        J2 pin 17            SCK       J2 pin 12
PCS        J1 pin 5             PCS1      J2 pin 6

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the example runs successfully, the following message is displayed in the terminal:

~~~~~~~~~~~~~~~~~~~~~
FLEXIO Master - LPSPI Slave interrupt example start.

This example use one flexio spi as master and one lpspi instance as slave on one board.

Master and slave are both use interrupt way.

Please make sure you make the correct line connection. Basically, the connection is:

FLEXI_SPI_master -- LPSPI_slave

      CLK        --    CLK

      PCS        --    PCS

      SOUT       --    SIN

      SIN        --    SOUT

This is LPSPI slave call back.

FLEXIO SPI master <-> LPSPI slave transfer all data matched!

End of example.
~~~~~~~~~~~~~~~~~~~~~
