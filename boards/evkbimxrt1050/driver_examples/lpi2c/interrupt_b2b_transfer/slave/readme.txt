Overview
========
The lpi2c_interrupt_b2b_transfer_slave example shows how to use lpi2c driver as slave to do board to board transfer 
with interrupt:

In this example, one lpi2c instance as slave and another lpi2c instance on the other board as master. Master sends a 
piece of data to slave, and receive a piece of data from slave. This example checks if the data received from 
slave is correct.
Hardware requirements
=====================
- Mini/micro USB cable
- Two EVKB-IMXRT1050 board
- Personal Computer

Board settings
==============
LPI2C one board:
  + Transfer data from MASTER_BOARD to SLAVE_BOARD of LPI2C interface, LPI2C1 pins of MASTER_BOARD are connected with
    LPI2C1 pins of SLAVE_BOARD
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MASTER_BOARD        CONNECTS TO         SLAVE_BOARD
Pin Name     Board Location     Pin Name     Board Location
LPI2C1_SCL   J23-5              LPI2C1_SCL   J23-5
LPI2C1_SDA   J23-6              LPI2C1_SDA   J23-6
GND          J24-7              GND          J24-7
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Prepare the Demo
================
1. Connect a mini USB cable between the PC host and the OpenSDA USB port on the board.
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
LPI2C board2board interrupt example -- Slave transfer.


End of LPI2C example .
~~~~~~~~~~~~~~~~~~~~~

Toolchain supported
===================
- IAR embedded Workbench  8.32.1
- Keil MDK  5.26
- GCC ARM Embedded  7.3.1
- MCUXpresso 10.3.0

