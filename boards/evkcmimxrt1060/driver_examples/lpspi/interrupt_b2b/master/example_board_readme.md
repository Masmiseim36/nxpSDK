Hardware requirements
=====================
- Mini/micro USB cable
- Two MIMXRT1060-EVKC board
- Personal Computer

Board settings
============
Remove the resistor R347,R348,R349,R351,R363,R364,R365.
Weld 0Ω resistor to R346,R350,R356,R362.

SPI two boards:
Transfer data from one board instance to another board's instance.
SPI0 pins are connected with SPI0 pins of another board
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
INSTANCE0(SPI0)     CONNECTS TO         INSTANCE0(SPI0)
Pin Name   Board Location     Pin Name  Board Location
SOUT        J17 pin 4           SIN       J17 pin 5
SIN         J17 pin 5           SOUT      J17 pin 4
SCK         J17 pin 6           SCK       J17 pin 6
PCS0        J17 pin 3           PCS0      J17 pin 3 
GND         J17 pin 7           GND       J17 pin 7
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



Note: Please ensure not to insert any SD CARD device on two boards

