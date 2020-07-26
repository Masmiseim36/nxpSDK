Overview
========
This example show application of FatFs with SD card controled over SPI interface. It mounts a file
system based on a SD card then does "creat directory/read directory/create file/write file/read file"
operation.


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
When the example runs successfully,you can see the similar information from the terminal as below.

~~~~~~~~~~~~~~~~~~~~~
FATFS example to demonstrate how to use FATFS with SD card over SPI.

Please inserts a card into board.
Detected SD card inserted.

Make file system......The time may be long if the card capacity is big.

Create directory......

Create a file in that directory......

List the file in that directory......
General file : F_1.DAT.
Directory file : DIR_2.

Write/read file until encounters error......

Write to above created file.
Read from above created file.
Compare the read/write content......
The read/write content is consistent.

Input 'q' to quit read/write.
Input other char to read/write file again.
~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

