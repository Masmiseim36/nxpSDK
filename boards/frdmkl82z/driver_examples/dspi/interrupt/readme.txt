Overview
========
The dspi_interrupt example shows how to use DSPI driver in interrupt way:

In this example , one dspi instance used as DSPI master and another dspi instance used as DSPI slave in the same board.
This example does not use the transactional API in DSPI driver. It's a demonstration that how to use the interrupt in KSDK driver.

1. DSPI master send/received data to/from DSPI slave in interrupt . (DSPI Slave using interrupt to receive/send the data)
Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-KL82Z board
- Personal Computer

Board settings
==============
SPI one board:
Transfer data from instance0 to instance 1 of SPI interface.
SPI0 pins are connected with SPI1 pins of board
For REV A board, connect the pins as bellow: 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
INSTANCE0(SPI0)     CONNECTS TO         INSTANCE1(SPI1)
Pin Name   Board Location     Pin Name  Board Location
SIN        J12 pin 6           SOUT      J22 pin 6
SOUT       J12 pin 13          SIN       J22 pin 7
SCK        J12 pin 18          SCK       J22 pin 5
PCS0       J12 pin 17          PCS0      J22 pin 4
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
For REV B board, connect the pins as bellow:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
INSTANCE0(SPI0)     CONNECTS TO         INSTANCE1(SPI1)
Pin Name   Board Location     Pin Name  Board Location
SIN        J12 pin 15          SOUT      J22 pin 6
SOUT       J12 pin 16          SIN       J22 pin 7
SCK        J12 pin 18          SCK       J22 pin 5
PCS0       J12 pin 17          PCS0      J22 pin 4
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
When the example runs successfully, the following message is displayed in the terminal:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
dspi_functional_interrupt example start.
This example use one dspi instance as master and another as slave on one board.
Master and slave are both use interrupt way.
Please make sure you make the correct line connection. Basically, the connection is:

DSPI_master -- DSPI_slave

   CLK      --    CLK
   PCS      --    PCS
   SOUT     --    SIN
   SIN      --    SOUT

DSPI transfer all data matched!
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Toolchain supported
===================
- IAR embedded Workbench  8.32.1
- Keil MDK  5.26
- GCC ARM Embedded  7.3.1
- MCUXpresso 10.3.0

