Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT1160-EVK board
- Personal Computer

Board settings
============
Remove 0Ω resistors R200,R404,R406,R408.

SPI two boards:
Transfer data from one board instance to another board's instance.
SPI1 pins are connected with SPI1 pins of another board
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
INSTANCE1(LPSPI1)     CONNECTS TO         INSTANCE1(LPSPI1)
Pin Name   Board Location     Pin Name  Board Location
SOUT        J10 pin 8           SIN       J10 pin 10
SIN         J10 pin 10          SOUT      J10 pin 8
SCK         J10 pin 12          SCK       J10 pin 12
PCS0        J10 pin 6           PCS0      J10 pin 6 
GND         J10 pin 14          GND       J10 pin 14
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Prepare the Demo
===============
1.  Connect a mini USB cable between the PC host and the OpenSDA USB port on the board.
2.  Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Reset the SoC and run the project.


