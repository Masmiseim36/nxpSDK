Overview
========
CMSIS-Driver defines generic peripheral driver interfaces for middleware making it reusable across a wide 
range of supported microcontroller devices. The API connects microcontroller peripherals with middleware 
that implements for example communication stacks, file systems, or graphic user interfaces. 
More information and usage methord please refer to http://www.keil.com/pack/doc/cmsis/Driver/html/index.html.

The cmsis_dspi_interrupt_transfer example shows how to use CMSIS DSPI driver in interrupt way:
 
In this example , one dspi instance used as master and another dspi instance used as slave in the same board.
1. DSPI master send/received data to/from DSPI slave in edma . (DSPI Slave using edma to receive/send the data)


Toolchain supported
===================
- IAR embedded Workbench 7.80.4
- Keil MDK 5.21a
- GCC ARM Embedded 2016-5.4-q3
- Kinetis Development Studio IDE 3.2.0
- MCUXpresso0.8

Hardware requirements
=====================
- Micro USB cable
- FRDM-KW41Z board
- Personal Computer

Board settings
==============
The connection should be set as following:
Transfer data from instance0 to instance 1 of SPI interface.
SPI0 pins are connected with SPI1 pins of board
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
INSTANCE0(SPI0)     CONNECTS TO         INSTANCE1(SPI1)
Pin Name   Board Location       Pin Name  Board Location
SOUT       J1  pin 6            SIN       J2  pin 5
SIN        J1  pin 7            SOUT      J2  pin 4
SCK        J1  pin 4            SCK       J2  pin 6
PCS0       J1  pin 3            PCS0      J2  pin 3
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
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the example runs successfully, you can see the similar information from the terminal as below.

CMSIS DSPI interrupt transfer example start.
This example use one dspi instance as master and another as slave on one board.
Master and slave are both use interrupt way.
Please make sure you make the correct line connection. Basically, the connection is:
DSPI_master -- DSPI_slave
   CLK      --    CLK
   PCS      --    PCS
   SOUT     --    SIN
   SIN      --    SOUT
This is SlaveSignalEvent.

DSPI transfer all data matched!

Customization options
=====================

