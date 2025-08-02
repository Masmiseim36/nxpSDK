Hardware requirements
=====================
- Micro USB cable
- Two MIMXRT1180-EVK boards
- Personal Computer

Board settings
============
Weld 1.5kΩ to R778.
I3C one board:
  + Transfer data from MASTER_BOARD to SLAVE_BOARD of I3C interface, I3C2 pins of MASTER_BOARD are connected with
    I3C2 pins of SLAVE_BOARD
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MASTER_BOARD        CONNECTS TO         SLAVE_BOARD
Pin Name   Board Location     Pin Name   Board Location
I3C2_CLK       J50-1           I3C2_CLK      J50-1
I3C2_SDA       J50-2           I3C2_SDA      J50-2
GND            J50-4           GND           J50-4
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.
