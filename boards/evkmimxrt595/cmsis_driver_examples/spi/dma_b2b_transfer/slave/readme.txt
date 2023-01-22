Overview
========
CMSIS-Driver defines generic peripheral driver interfaces for middleware making it reusable across a wide 
range of supported microcontroller devices. The API connects microcontroller peripherals with middleware 
that implements for example communication stacks, file systems, or graphic user interfaces. 
More information and usage method please refer to http://www.keil.com/pack/doc/cmsis/Driver/html/index.html.

The cmsis_spi_dma_b2b_transfer_slave example shows how to use spi driver as master to do board to board transfer 
with DMA:

In this example, one spi instance as master and another spi instance on the other board as slave. Master sends a 
piece of data to slave, and receive a piece of data from slave. This example checks if the data received from 
slave is correct.

Toolchain supported
===================
- IAR embedded Workbench  9.32.1
- Keil MDK  5.37
- GCC ARM Embedded  10.3.1
- MCUXpresso  11.7.0

Hardware requirements
=====================
- Micro USB cable
- EVK-MIMXRT595 board
- Personal Computer

Board settings
==============
Connect SPI master on board to SPI slave on other board
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Master_board(SPI5)           Slave_board(SPI5)                          
Pin Name   Board Location     Pin Name   Board Location                     
MISO       JP26 pin 3          MISO      JP26 pin 3  
MOSI       JP26 pin 2          MOSI      JP26 pin 2  
SCK        JP26 pin 4          SCK       JP26 pin 4  
PCS0       JP26 pin 1          PCS0      JP26 pin 1  
GND        JP26 pin 5          GND       JP26 pin 5  
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Prepare the Demo
================
1.  Connect a micro USB cable between the PC host and the CMSIS DAP USB port (J40) on the board
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the demo runs successfully, the log would be seen on the terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CMSIS SPI board2board DMA example -- Slave transfer.


 Slave example is running...


 Slave receive:
      0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
     10 11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F
     20 21 22 23 24 25 26 27 28 29 2A 2B 2C 2D 2E 2F
     30 31 32 33 34 35 36 37 38 39 3A 3B 3C 3D 3E 3F
Succeed!
~~~~~~~~~~~~~~~~~~~~~~~~~~~~
