Overview
========
The lpspi_interrupt example shows how to use LPSPI driver in interrupt way:

In this example , one lpspi instance used as LPSPI master and another lpspi instance used as LPSPI slave.

1. LPSPI master send/received data to/from LPSPI slave in interrupt . (LPSPI Slave using interrupt to receive/send the data)

The example supports board to board connection.

With board to board connection, one LPSPI instance on one board is used as LPSPI master and the LPSPI instance on other board is used as LPSPI slave. Tasks are created to run on each board to handle LPSPI communication.
    File freertos_lpspi.c should have following definitions:
    #define EXAMPLE_CONNECT_SPI BOARD_TO_BOARD
    For board used as LPSPI master:
        #define SPI_MASTER_SLAVE isMASTER
    For board used as LPSPI slave:
        #define SPI_MASTER_SLAVE isSLAVE


Toolchain supported
===================
- IAR embedded Workbench  8.32.3
- Keil MDK  5.27
- MCUXpresso  11.0.1
- GCC ARM Embedded  8.2.1

Hardware requirements
=====================
- Mini USB cable
- FRDM-K32L3A6 boards
- Personal Computer

Board settings
==============
To make the example work, connections needed to be as follows:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MASTER(SPI0)           connect to        SLAVE(SPI3)
Pin Name   Board Location     Pin Name  Board Location
SOUT       J2 pin 8             SIN       J1  pin 6
SIN        J2 pin 10            SOUT      J1  pin 2
SCK        J2 pin 12            SCK       J3  pin 5
PCS2       J2 pin 6             PCS1      J3  pin 9
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Prepare the Demo
================
1. Connect a mini USB cable between the PC host and the OpenSDA USB port on the board.
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
The following message shows in the terminal if the example runs successfully.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~
FreeRTOS LPSPI example start.

This example use one lpspi instance as master and another as slave on a single board.

Master and slave are both use interrupt way.

Please make sure you make the correct line connection. Basically, the connection is:

LPSPI_master -- LPSPI_slave

    CLK      --    CLK

    PCS0     --    PCS0

    SOUT     --    SIN

    SIN      --    SOUT

LPSPI master transfer completed successfully.

LPSPI slave transfer completed successfully.

LPSPI transfer all data matched !
~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

