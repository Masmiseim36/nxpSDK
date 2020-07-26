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
- IAR embedded Workbench 8.11.3
- Keil MDK 5.23
- GCC ARM Embedded 6-2017-q2
- Kinetis Development Studio IDE 3.2.0
- MCUXpresso10.1.0

Hardware requirements
=====================
- Mini/micro USB cable
- TWR-K21D50M board
- TWR-MEM board
- Personal Computer

Board settings
==============
The example is configured to use UART1 with PTE16 and PTE17 pins.

The SPI1 is configured to communicate with SDCARD on the TWR-MEM board. The pin function is:
-PTB10:SPI1_PCS0;
-PTB11:SPI1_SCK;
-PTB16:SPI1_SOUT;
-PTB17:SPI1_SIN;

The example use PTE6 as card detection pin. In order to make the card detection pin work, the
jumper on TWR-MEM board should be set as follows:
-J12:3-4;

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

