Overview
========
CMSIS-Driver defines generic peripheral driver interfaces for middleware making it reusable across a wide 
range of supported microcontroller devices. The API connects microcontroller peripherals with middleware 
that implements for example communication stacks, file systems, or graphic user interfaces. 
More information and usage methord please refer to http://www.keil.com/pack/doc/cmsis/Driver/html/index.html.

The cmsis_spi_dma_transfer example shows how to use CMSIS SPI driver in dma way:
In this example , one spi instance used as master and another spi instance used as slave in the same board.
SPI master send/received data to/from SPI slave in dma . (SPI Slave using dma to receive/send the data)


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
- FRDM-KL26Z board
- Personal Computer

Board settings
==============
SPI one board:
Transfer data from instance0 to instance 1 of SPI interface, SPI0 pins are connected with
SPI1 pins of board
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
INSTANCE0        CONNECTS TO         INSTANCE1
Pin Name   Board Location     Pin Name  Board Location
MOSI       J1 pin 9           MOSI      J2 pin 8
MISO       J1 pin 11          MISO      J2 pin 10
SCK        J4 pin 9           SCK       J2 pin 12
PCS0       J1 pin 7           PCS0      J2 pin 6
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Prepare the Demo
================
1.  Connect a mini USB cable between the PC host and the OpenSDA USB port on the board.
2.  Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Reset the SoC and run the project.

Running the demo
================
When the example runs successfully, you can see the similar information from the terminal as below.
SPI CMSIS DMA transfer example start.
This example use one spi instance as master and another as slave on one board.
Master use DMA way , slave uses interrupt.
Please make sure you make the correct line connection. Basically, the connection is: 
SPI_master -- SPI_slave   
   CLK      --    CLK  
   PCS      --    PCS  
   MISO     --    MISO  
   MOSI     --    MOSI 
This is SlaveSignalEvent.
This is MasterSignalEvent.
 
SPI transfer all data matched! 
Customization options
=====================

