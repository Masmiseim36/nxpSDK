Hardware requirements
=====================
- Micro USB cable
- Two MIMXRT700-EVK boards
- Personal Computer

Board settings
============
1. Make sure JP12 is installed
2. SPI two boards:
Transfer data from one board instance to another board's instance.
SPI14 pins are connected with SPI14 pins of another board
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
INSTANCE0(SPI14)     CONNECTS TO         INSTANCE0(SPI14)
Pin Name   Board Location           Pin Name  Board Location
SOUT        J20 pin 3                   SIN       J20 pin 5
SIN         J20 pin 5                   SOUT      J20 pin 3
SCK         J20 pin 7                   SCK       J20 pin 7
PCS0        J20 pin 1                   PCS0      J20 pin 1
GND         J20 pin 9                   GND       J20 pin 9
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Prepare the Demo
===============
1.  Connect a mini USB cable between the PC host and the MCU-LINK USB port on the board.
2.  Open a serial terminal on PC for MCU-LINK serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Reset the SoC and run the project.

