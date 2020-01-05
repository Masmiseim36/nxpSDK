Overview
========
This example show application of FatFs with SD card controled over SPI interface. It mounts a file
system based on a SD card then does "creat directory/read directory/create file/write file/read file"
operation.


Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- Keil MDK  5.29
- GCC ARM Embedded  8.3.1
- MCUXpresso  11.1.0

Hardware requirements
=====================
- Mini/micro USB cable
- TWR-K64F120M board
- Personal Computer

Board settings
==============
The example is configured to use UART1 with PTC3 and PTC4 pins.

The SPI1 is configured to communicate with SDCARD through on board sdcard socket. The pin function is:
-PTE4:SPI1_PCS1;
-PTE2:SPI1_SCK;
-PTE3:SPI1_SOUT;
-PTE1:SPI1_SIN;


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

~~~~~~~~~~~~~~~~~~~~~
When the example runs successfully, you can see the similar information from the terminal as below.
FATFS example to demonstrate how to use FATFS with SD card over SPI. 

Please inserts a card into board. 
Detected SD card inserted.

Make file system......The time may be long if the card capacity is big.

Create directory......

Create a file in that directory......

Create a directory in that directory......

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

