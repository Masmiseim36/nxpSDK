Overview
========
The SDCARD FATFS project is a demonstration program that uses the SDK software. Tt mounts a file 
system based on a SD card then does "creat directory/read directory/create file/write file/read file"
operation. The file sdhc_config.h has default SDHC configuration which can be adjusted to let card
driver has different performance. The purpose of this example is to show how to use SDCARD driver 
based FATFS disk in SDK software.

Toolchain supported
===================
- IAR embedded Workbench 7.80.4
- GCC ARM Embedded 2016-5.4-q3

Hardware requirements
=====================
- Micro USB cable
- MCIMX6UL-EVK 
- Jlink
- Personal Computer

Board settings
==============
Please insert the SDCARD into card slot(P1701)

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
5.  Reset the SoC and run the project.

Running the demo
================
When the demo runs successfully, the log would be seen on the terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

FATFS example to demonstrate how to use FATFS with SD card.

Please insert a card into board.
Detected SD card inserted.

Makes file system......This time may be long if the card capacity is big.

Creates directory......

Creates a file in that directory......

Lists the file in that directory......
Directory file : ..
Directory file : ...
General file : F_1.DAT.

Writes/reads file until encounters error......

Writes to above created file.
Reads from above created file.
Compares the read/write content......
The read/write content is consistent.
Input 'q' to quit read/write.
Input other char to read/write file again.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

