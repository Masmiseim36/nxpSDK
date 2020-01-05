Overview
========
The SDCARD FatFs FreeRTOS project is a demonstration program that uses the SDK software. It reads/writes
/erases the SD card continuously. The purpose of this example is to show how to use SDCARD driver
with FatFs and freeRTOS in SDK software to access SD card.

Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- Keil MDK  5.29
- MCUXpresso  11.1.0
- GCC ARM Embedded  8.3.1

Hardware requirements
=====================
- Mini/micro USB cable
- TWR-K65F180M board
- Personal Computer
- SD card

Board settings
==============
The SDCARD Interrupt example is configured to use SDHC0 with PTE0, PTE1, PTE2, PTE3, PTE4, PTE5 pins
and use PTA9 pin as card detection pin. It doesn't detetet write protect switch state so don't open
the write protect switch on the SD card socket. It also uses UART2 with PTE16 and PTE17 pins. To make
OpenSDA serial device work, the jumpers should be set as following:
- J33: 2-3 connected
- J34: 2-3 connected

Prepare the Demo
================
1. Connect a USB cable between the PC host and the OpenSDA USB port (J2) on the board.
2. Open a serial terminal on PC for OpenSDA serial device with these settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Insert sdcard into card slot.
5. Either press the reset button on your board or launch the debugger in your IDE to begin running
   the example.

Running the demo
================
When the example runs successfully, you can see the similar information from the terminal as below.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SDCARD fatfs freertos example.

Card inserted.

Make file system......The time may be long if the card capacity is big.

Create directory......
TASK1: write file successed.
TASK1: file access is blocking.
TASK1: file access is blocking.
TASK2: write file successed.
TASK2: file access is blocking.
TASK2: file access is blocking.
TASK1: finsied.
TASK2: finsied.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

