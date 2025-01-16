Hardware requirements
=====================
- Mini/micro USB cable
- Two MIMXRT1060-EVKB board
- Personal Computer

Board settings
============
LPI2C one board:
  + Transfer data from MASTER_BOARD to SLAVE_BOARD of LPI2C interface, LPI2C1 pins of MASTER_BOARD are connected with
    LPI2C1 pins of SLAVE_BOARD
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MASTER_BOARD        CONNECTS TO         SLAVE_BOARD
Pin Name     Board Location     Pin Name     Board Location
LPI2C1_SCL   J33-6              LPI2C1_SCL   J33-6
LPI2C1_SDA   J33-5              LPI2C1_SDA   J33-5
GND          J17-7              GND          J17-7
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
