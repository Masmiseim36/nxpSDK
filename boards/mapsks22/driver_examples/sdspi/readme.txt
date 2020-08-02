Overview
========

The SDSPI example shows how to use SDSPI driver to initializes SD card and read/write data blocks 
of SD card. It calls read/write APIs of the driver to read/write single and multiple data blocks
of SD card forever until encounters some error to result in the read data content is different the 
original data content written to the card.

The example only uses the polling transaction API of SPI driver to send/receive data 
over SPI bus. More detail transaction API of SPI driver can refer to the SPI driver and SPI example.


Toolchain supported
===================
- Keil MDK 5.24a
- IAR embedded Workbench 8.22.2
- GCC ARM Embedded 7-2017-q4-major
- MCUXpresso10.2.0

Hardware requirements
=====================
- Mini/micro USB cable
- MAPS-KS22F256 board & MAPS-DOCK board
- Personal Computer

Board settings
==============
The SDSPI example is configured to use SPI1 as master to communicate with SDCARD on the MAPS-DOCK
board as slave. SPI1 on the MAPS-KS22F256 board is configured to use PTC3 as PCS pin, PTC5 as SCK
pin, PTD3 as MISO pin, PTD2 as MOSI pin.

The example use PTC0 as card detection pin.

To make SDCARD on the MAPS-DOCK board work, the jumpers of JP8, JP9 and JP13 on the MAPS-DOCK board
should be removed.

It also uses UART1 with PTE0 and PTE1 pins. To make serial device work, the jumpers of JP7 on
the MAPS-DOCK board should be connected.

Prepare the Demo
================
1. Connect a mini USB cable between the PC host and the USB port (CN14) on MAPS-DOCK board.
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
When the example runs successfully,you can see the similar information from the terminal as below:

SD card over SPI example start.

Please insert the SD card
Detected SD card inserted.

Read/write SD card continuously until encounters error.

Write/read one data block......
Compare the read/write content......
The read/write content is consistent.

Write/read multiple data blocks......
Compare the read/write content......
The read/write content is consistent.

Input 'q' to quit read/write process.
Input other char to read/write data blocks again.
Customization options
=====================

