Overview
========
The ecspi_sdma_b2b_transfer example shows how to use ECSPI driver in sdma way:

In this example , we need two boards, one board used as ECSPI master and another board used as ECSPI slave.
The file 'ecspi_sdma_b2b_transfer_slave.c' includes the ECSPI slave code.
This example uses the transactional API in ECSPI driver.

1. ECSPI master send/received data to/from ECSPI slave in sdma . (ECSPI Slave using sdma to receive/send the data)

Toolchain supported
===================
- IAR embedded Workbench 7.80.4
- GCC ARM Embedded 2016-5.4-q3

Hardware requirements
=====================
- Micro USB cable
- MCIMX6ULL-EVK -CM board and MCIMX6UL-BB board
- JLink Plus
- 5V power supply
- Personal Computer

Board settings
==============
ECSPI board to board:
Before start this example, please make sure the R1724/R1725/R1726/R1727 are jointed and the 
R1633 is unjointed on board MCIMX6UL-BB due to the the pin multiplexing. 
Transfers data through the instance 4 of the ECSPI interface. ECSPI4 pins of the master board are
connected to the ECSPI4 pins of the slave board.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MASTER BOARD           CONNECTS TO         SLAVE BOARD
Pin Name    Board Location               Pin Name  Board Location
MISO        J1704 - 5                       MISO      J1704 - 5
MOSI        J1704 - 4                       MOSI      J1704 - 4
SCK         J1704 - 6                       SCK       J1704 - 6
SS0         J1704 - 3                       SS0       J1704 - 3
GND         J1704 - 7                       GND       J1704 - 7
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Prepare the Demo
================
1.  Connect 5V power supply and JLink Plus to the board, switch SW2001 to power on the board.
2.  Connect a USB cable between the host PC and the J1901 USB port on the target board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the demo runs successfully, the log would be seen on the debug terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ECSPI board to board sdma example.

 Slave example is running...
 
 Slave starts to receive data!
 This is ECSPI slave transfer completed callback.
 It's a successful transfer.
 
 Slave starts to transmit data!
 This is ECSPI slave transfer completed callback.
 It's a successful transfer.
 
 Slave receive:
      1  2  3  4  5  6  7  8  9  A  B  C  D  E  F 10
     11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F 20
     21 22 23 24 25 26 27 28 29 2A 2B 2C 2D 2E 2F 30
     31 32 33 34 35 36 37 38 39 3A 3B 3C 3D 3E 3F 40

 Slave example is running...

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

