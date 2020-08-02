Overview
========
This example show application of FatFs with SD card controled over SPI interface. It mounts a file
system based on a SD card then does "creat directory/read directory/create file/write file/read file"
operation.


Toolchain supported
===================
- Keil MDK 5.24a
- IAR embedded Workbench 8.22.2
- GCC ARM Embedded 7-2017-q4-major
- MCUXpresso10.2.0

Hardware requirements
=====================
- Mini/micro USB cable
- MAPS-KS22F256 board
- MAPS-DOCK board
- Personal Computer

Board settings
==============
The SDSPI_FATFS example is configured to use SPI1 as master to communicate with SDCARD on the MAPS-DOCK
board as slave.SPI1 on the MAPS-KS22F256 board is configured to use PTC3 as PCS pin, PTC5 as SCK
pin, PTD3 as MISO pin, PTD2 as MOSI pin.

The example use PTC0 as card detection pin.

To make SDCARD on the MAPS-DOCK board work, the jumpers of JP8, JP9 and JP13 on the MAPS-DOCK board
should be removed.

It also uses UART1 with PTE0 and PTE1 pins. To make serial device work, the jumpers of JP7 on
the MAPS-DOCK board should be connected.

Prepare the Demo
================
1. Connect a mini USB cable between the PC host and the OpenSDA USB port (J2) on twrk64f120m board.
2. Open a serial terminal on PC for OpenSDA serial device with these settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running
   the example.

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
1. <FIXME>The USE_STDIO_FUNCTIONS definition determines whether the example uses standard I/O
   functions, such as printf. If it is not defined, then the example accesses the UART driver directly.

