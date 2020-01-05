Overview
========
The lpi2c_edma_b2b_transfer_slave example shows how to use lpi2c driver as slave to do board to board transfer 
with a EDMA master:

In this example, one lpi2c instance as slave and another lpi2c instance on the other board as master. Master sends a 
piece of data to slave, and receive a piece of data from slave. This example checks if the data received from 
slave is correct.

Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- Keil MDK  5.29
- GCC ARM Embedded  8.3.1
- MCUXpresso  11.1.0

Hardware requirements
=====================
- Micro USB cable
- Two TWR-KE18F boards
- Personal Computer
- Two Primary Elevator Towers

Board settings
==============
I2C one board:
  + Transfer data from MASTER_BOARD to SLAVE_BOARD of I2C interface, I2C0 pins of MASTER_BOARD are connected with
    I2C0 pins of SLAVE_BOARD
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MASTER_BOARD        CONNECTS TO         SLAVE_BOARD
Pin Name   Primary Elevator   Pin Name   Primary Elevator
I2C0_SCL   A7                 I2C0_SCL   A7
I2C0_SDA   B8                 I2C0_SDA   A8
GND        B2                 GND        B2
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
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the example runs successfully, the following message is displayed in the terminal:

~~~~~~~~~~~~~~~~~~~~~
LPI2C board2board EDMA example -- Slave transfer.


End of LPI2C example .
~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

