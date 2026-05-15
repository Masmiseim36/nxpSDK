Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-IMXRT1186 board
- Personal Computer

Board settings
============
SPI two boards:
Transfer data from one board instance to another board's instance.
SPI2 pins are connected with SPI2 pins of another board
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
INSTANCE(LPSPI2)     CONNECTS TO    INSTANCE(LPSPI2)
Pin Name   Board Location   Pin Name   Board Location
SOUT       J3-16           SIN        J3-14
SIN        J3-14           SOUT       J3-16
SCK        J3-18           SCK        J3-18
PCS0       J3-12           PCS0       J3-12
GND        J2-19           GND        J2-19
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


