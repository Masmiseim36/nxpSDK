Overview
========
The spi_interrupt_b2b_master example shows how to use spi functional API to do interrupt transfer as a master:

In this example, the spi instance as master. Master sends a piece of data to slave, receive data from slave,and check
if the data master received is correct. This example needs to work with spi_interrupt_b2b_slave example.

Toolchain supported
===================
- IAR embedded Workbench  9.10.2
- Keil MDK  5.34
- GCC ARM Embedded  10.2.1
- MCUXpresso  11.4.0

Hardware requirements
=====================
- Micro USB cable
- Two TWR-KM35Z75M board
- Personal Computer
- Two Elevator Tower

Board settings
==============
SPI one board:
  + Transfer data from MASTER_BOARD to SLAVE_BOARD of SPI interface, SPI0 pins of MASTER_BOARD are connected with
    SPI0 pins of SLAVE_BOARD
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MASTER_BOARD        CONNECTS TO          SLAVE_BOARD
Pin Name   Board Location     Pin Name   Board Location
MOSI       B-10               MOSI       B-10
MISO       B-11               MISO       B-11
SCK        B-7                SCK        B-7
PCS0       B-9                PCS0       B-9
GND        B-2                GND        B-2
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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
4.  Reset the SoC and run the project.

Running the demo
================
When the example runs successfully, the following message is displayed in the terminal:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SPI board to board interrupt slave example started!
SPI transfer finished!
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
