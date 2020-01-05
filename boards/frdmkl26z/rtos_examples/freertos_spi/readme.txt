Overview
========
The freertos_spi example shows how to use SPI driver in FreeRTOS:

In this example , one spi instance is used as SPI master with blocking and another spi instance is used as SPI slave.

1. SPI master sends/receives data using task blocking calls to/from SPI slave. (SPI Slave uses interrupt to receive/send
the data)


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
  + Transfer data loopback with SPI0 module, only connect MOSI pin to MISO pin of SPI0 on board
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
INSTANCE0   
Pin Name   Board Locatio        Pin Name    Board Location
MOSI       J1 pin 9      <==>   MISO        J1 pin 11
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
When the example runs successfully, you can see the similar information on the terminal as below.

SPI FreeRTOS example start.
This example use one SPI instance in master mode
to transfer data through loopback
Please be sure to externally connect together SOUT and SIN signals.
   SOUT     --    SIN  
SPI transfer completed successfully. 
Data verified ok.

Customization options
=====================

