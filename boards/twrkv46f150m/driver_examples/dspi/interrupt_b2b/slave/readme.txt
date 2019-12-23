Overview
========
The dspi_interrupt_b2b example shows how to use DSPI driver in interrupt way:

In this example , we need two boards , one board used as DSPI master and another board used as DSPI slave.
The file 'dspi_interrupt_b2b_slave.c' includes the DSPI slave code.
This example does not use the transactional API in DSPI driver. It's a demonstration that how to use the interrupt in KSDK driver.

1. DSPI master send/received data to/from DSPI slave in interrupt . (DSPI Slave using interrupt to receive/send the data)

Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- Keil MDK  5.29
- GCC ARM Embedded  8.3.1
- MCUXpresso  11.1.0

Hardware requirements
=====================
- Mini/micro USB cable
- Two board TWR-KV46F150M
- Personal Computer

Board settings
==============
The connection should be set as following:
To use UART on board, make sure that J505-3 connects to J505-4, J506-3 connects to J506-4
Transfers data through the instance 0 of the SPI interface. SPI0 pins of the master board are
connected to the SPI0 pins of the slave board.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MASTER BOARD           CONNECTS TO         SLAVE BOARD
Pin Name   Primary Elevator               Pin Name  Primary Elevator
MISO       B44                            MISO      B45
MOSI       B45                            MOSI      B44
SCK        B48                            SCK       B48
PCS0       B46                            PCS0      B46
GND        B2                             GND       B2
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
When the demo runs successfully, the log would be seen on the OpenSDA terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 DSPI board to board functional interrupt example.

 Slave example is running...

 Slave receive below data in 1st round transfer:
      1  2  3  4  5  6  7  8  9  A  B  C  D  E  F 10
     11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F 20
     21 22 23 24 25 26 27 28 29 2A 2B 2C 2D 2E 2F 30
     31 32 33 34 35 36 37 38 39 3A 3B 3C 3D 3E 3F 40

 Slave example is running...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

