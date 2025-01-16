Hardware requirements
=====================
- Mini/micro USB cable
- Two EVK-MIMXRT1064 board
- Personal Computer

Board settings
============
Weld 0Ω resistor to R278,R279,R280,R281.

SPI one board:
Transfer data from one board instance to another board's instance.
SPI0 pins are connected with SPI0 pins of another board
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
INSTANCE0(SPI0)     CONNECTS TO         INSTANCE0(SPI0)
Pin Name   Board Location     Pin Name  Board Location
SOUT        J24 pin 4           SIN       J24 pin 5
SIN         J24 pin 5           SOUT      J24 pin 4
SCK         J24 pin 6           SCK       J24 pin 6
PCS0        J24 pin 3           PCS0      J24 pin 3 
GND         J24 pin 7           GND       J24 pin 7
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


