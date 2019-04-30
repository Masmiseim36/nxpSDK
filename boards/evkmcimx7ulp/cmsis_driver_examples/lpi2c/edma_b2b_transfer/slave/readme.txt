Overview
========
CMSIS-Driver defines generic peripheral driver interfaces for middleware making it reusable across a wide 
range of supported microcontroller devices. The API connects microcontroller peripherals with middleware 
that implements for example communication stacks, file systems, or graphic user interfaces. 
More information and usage method please refer to http://www.keil.com/pack/doc/cmsis/Driver/html/index.html.

The cmsis_i2c_edma_b2b_transfer_master example shows how to use i2c driver as master to do board to board transfer 
with EDMA:

In this example, one i2c instance as master and another i2c instance on the other board as slave. Master sends a 
piece of data to slave, and receive a piece of data from slave. This example checks if the data received from 
slave is correct.

Toolchain supported
===================
- IAR embedded Workbench  8.32.3
- GCC ARM Embedded  8.2.1

Hardware requirements
=====================
- Micro USB cable
- two MCIMX7ULP-EVK boards
- J-Link Debug Probe
- 5V power supply
- Personal Computer

Board settings
==============
LPI2C one board:
  + Transfer data from MASTER_BOARD to SLAVE_BOARD of LPI2C interface, LPI2C3 pins of MASTER_BOARD are connected with
    LPI2C3 pins of SLAVE_BOARD
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MASTER_BOARD        CONNECTS TO         SLAVE_BOARD
Pin Name     Board Location     Pin Name     Board Location
I2C3_SCL     TP24               I2C3_SCL     TP24
I2C3_SDA     TP26               I2C3_SDA     TP26
GND                             GND
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#### Please note this application can't support running with Linux BSP! ####

Prepare the Demo
================
1.  Connect 5V power supply and J-Link Debug Probe to the board, switch SW1 to power on the board.
2.  Connect a micro USB cable between the host PC and the J6 USB port on the target board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the example

Running the demo
================
The following message shows in the terminal if the example runs successfully.
~~~~~~~~~~~~~~~~~~~~~
CMSIS LPI2C board2board EDMA example -- Slave transfer.


End of LPI2C example .
~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

