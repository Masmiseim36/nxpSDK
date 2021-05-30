Overview
========
The spi_dma_board2board_master example shows how to use spi driver as master to do board to board transfer with DMA:

In this example, one spi instance as master and another spi instance on othereboard as slave. Master sends a piece of
data to slave, and receive a piece of data from slave. This example checks if the data received from slave is correct.

Toolchain supported
===================
- IAR embedded Workbench 7.80.4
- Keil MDK 5.21a
- GCC ARM Embedded 2016-5.4-q3
- Kinetis Development Studio IDE 3.2.0
- MCUXpresso0.8

Hardware requirements
=====================
- Mini USB cable
- Two TWR-KM34Z75M board
- Personal Computer
- Two Elevator Tower

Board settings
==============
SPI one board:
  + Transfer data from MASTER_BOARD to SLAVE_BOARD of SPI interface, SPI0 pins of MASTER_BOARD are connected with
    SPI0 pins of SLAVE_BOARD
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MASTER_BOARD        CONNECTS TO          SLAVE_BOARD
Pin Name   Board Location     Pin Name   Board Location
MOSI       B-10               MOSI       B-10
MISO       B-11               MISO       B-11
SCK        B-7                SCK        B-7
PCS0       B-9                PCS0       B-9
GND        B-2                GND        B-2
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
4.  Reset the SoC and run the project.

Running the demo
================
When the example runs successfully, the following message is displayed in the terminal:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Master Start...

Succeed!
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

