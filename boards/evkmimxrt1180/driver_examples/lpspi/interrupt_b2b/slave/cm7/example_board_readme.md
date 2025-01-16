Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT1180-EVK board
- Personal Computer

Board settings
============
SPI two boards:
Transfer data from one board instance to another board's instance.
SPI3 pins are connected with SPI3 pins of another board
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
INSTANCE(LPSPI3)     CONNECTS TO    INSTANCE(LPSPI3)
Pin Name   Board Location   Pin Name   Board Location
SOUT       J44-8            SIN        J44-10
SIN        J44-10           SOUT       J44-8
SCK        J44-12           SCK        J44-12
PCS0       J44-6            PCS0       J44-6 
GND        J44-14           GND        J44-14
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


