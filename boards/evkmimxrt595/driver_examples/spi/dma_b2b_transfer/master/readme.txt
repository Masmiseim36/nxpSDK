Overview
========
The spi_dma_b2b_transfer_master example shows how to use driver API to transfer in DMA way.  

In this example, one spi instance as master and another spi instance on the other board as slave. 
Master sends a piece of data to slave, and receive a piece of data from slave. This example checks
if the data received from slave is correct. This example needs to work with spi_dma_b2b_transfer_slave 
example.

Note: This example will run in master mode, please prepare another board for slave, and the slave 
      board should be started first.

Project Infomation
1.How to set the baudrate of SPI?
  The baudrate of SPI is easy to set, just make sure the source clock of SPI.
  The formula is below:
  
  baudrate = PCLK_SPIn / DIVVAL
  SPIn->DIV = DIVVAL -1
  For example, if the needed baudrate of SPI is 500khz, and the SPI clock is 12MHz
  DIVVAL = 12000000/500000
  DIVVAL = 24
  SPIn->DIV = DIVVAL -1 = 23

Program Flow:
Main routine:
  1.Initialize the hardware.
	Configure pin settings, clock settings by using BOARD_InitHardware();
	
  2.Send information to terminal by using debug console.
	
  3.Initialize the SPI with configuration.
	
  4.Set up DMA configuration for master SPI.
    Initialize DMA and DMA channel setting(create handle and set callback) for both
    SPI RX and TX, set prioritory for TX channel and RX channel.
	
  5.Start SPI master transfer in DMA way.
    Initialize buffers and start SPI transfer in DMA way.
	
  6.Check if data from master is all right.
    Waiting for transmission complete, print received data and log information to terminal.
  
  7.De-initialize the SPI and DMA.

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
The example provides 2 options of SPI interfaces in communication.
1. SPI5 (default, normal SPI: #define USE_HS_SPI 0)
2. SPI14 (optional, high speed SPI: #define USE_HS_SPI 1)
User can change the USE_HS_SPI value in spi_dma_b2b_transfer_master.c to choose different SPI interface.

Connect SPI master on board to SPI slave on other board. Only 1 set of signals need to be connected, either
SPI5 or SPI14 depending on the USE_HS_SPI setting.
NOTE: To use SPI14 on EVK board, please connect JP32 1-2.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Master_board(SPI14)             Slave_board(SPI14) 
Pin Name   Board Location     Pin Name   Board Location
MISO       J36 pin 5          MISO       J36 pin 5
MOSI       J36 pin 3          MOSI       J36 pin 3
SCK        J36 pin 7          SCK        J36 pin 7
PCS0       J36 pin 1          PCS0       J36 pin 1
GND        J36 pin 9          GND        J36 pin 9
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
This is SPI DMA tranfer master example.
This example will communicate with another slave SPI on the other board.
Please start the slave board first!

The received data are:
    0x00  0x01  0x02  0x03  0x04  0x05  0x06  0x07  0x08  0x09  0x0A  0x0B  0x0C  0x0D  0x0E  0x0F
    0x10  0x11  0x12  0x13  0x14  0x15  0x16  0x17  0x18  0x19  0x1A  0x1B  0x1C  0x1D  0x1E  0x1F
    0x20  0x21  0x22  0x23  0x24  0x25  0x26  0x27  0x28  0x29  0x2A  0x2B  0x2C  0x2D  0x2E  0x2F
    0x30  0x31  0x32  0x33  0x34  0x35  0x36  0x37  0x38  0x39  0x3A  0x3B  0x3C  0x3D  0x3E  0x3F
SPI transfer all data matched!

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
