Overview
========
The i2c_polling_b2b_transfer_slave example shows how to use i2c driver as slave to do board to board transfer 
with a polling master:

In this example, one i2c instance as slave and another i2c instance on the other board as master. Master sends a 
piece of data to slave, and receive a piece of data from slave. This example checks if the data received from 
slave is correct.

Toolchain supported
===================
- IAR embedded Workbench 7.80.4
- GCC ARM Embedded 2016-5.4-q3

Hardware requirements
=====================
- Micro USB cable
- MCIMX6UL-EVK  board
- JLink Plus
- 5V power supply
- Personal Computer

Board settings
==============
I2C one board:
  + Transfer data from SLAVE_BOARD to MASTER_BOARD of I2C interface, I2C2 pins of SLAVE_BOARD are connected with
    I2C2 pins of MASTER_BOARD
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SLAVE_BOARD        CONNECTS TO          MASTER_BOARD
Pin Name   Board Location     Pin Name   Board Location
I2C2_SCL       J1704-10       I2C2_SCL      J1704-10
I2C2_SDA       J1704-9        I2C2_SDA      J1704-9
GND            J1704-7        GND           J1704-7
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the demo runs successfully, the following message is displayed in the terminal:

I2C board2board polling example -- Slave transfer.


End of I2C example .

Customization options
=====================

