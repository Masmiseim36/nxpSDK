Overview
========
The spi_interrupt_b2b_transfer example shows how to use spi CMSIS driver in interrupt way:

In this example , we need two boards, one board used as spi master and another board used as spi slave.
The file 'spi_interrupt_b2b_transfer_master.c' includes the spi master code.
This example uses the transactional API in spi driver.

1. spi master send/received data to/from spi slave in interrupt . 

Toolchain supported
===================
- IAR embedded Workbench 8.11.3
- Keil MDK 5.23
- GCC ARM Embedded 6-2017-q2
- Kinetis Development Studio IDE 3.2.0
- MCUXpresso10.1.0

Hardware requirements
=====================
- Mini USB cable
- FRDM-KL03Z board
- Personal Computer

Board settings
==============
SPI one board:
  + Transfer data from MASTER_BOARD to SLAVE_BOARD of SPI interface, SPI0 pins of MASTER_BOARD are connected with
    SPI0 pins of SLAVE_BOARD
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
           MASTER_BOARD    CONNECTS TO   SLAVE_BOARD
Pin Name   Board Location                Board Location
MOSI       J2-4                          J2-4
MISO       J2-5                          J2-5
SS         J2-3                          J2-3
CLK        J2-6                          J2-6
GND        J2-7                          J2-7
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 9600 baud rate
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
SPI CMSIS driver board to board interrupt example.
This example use one board as master and another as slave.
Master and slave uses interrupt way. Slave should start first. 
Please make sure you make the correct line connection. Basically, the connection is: 
SPI_master -- SPI_slave   
   CLK      --    CLK  
   PCS      --    PCS 
   SOUT     --    SIN  
   SIN      --    SOUT 
   GND      --    GND 

 Master transmit:

  0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
 10 11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F
 20 21 22 23 24 25 26 27 28 29 2A 2B 2C 2D 2E 2F
 30 31 32 33 34 35 36 37 38 39 3A 3B 3C 3D 3E 3F
 
SPI transfer all data matched! 

 Master received:

  0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
 10 11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F
 20 21 22 23 24 25 26 27 28 29 2A 2B 2C 2D 2E 2F
 30 31 32 33 34 35 36 37 38 39 3A 3B 3C 3D 3E 3F
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

