Overview
========
The flexio_i2s_interrupt example shows how to use flexio_i2s driver with interrupt:

In this example, flexio acts as I2S module to record data from line-in line and playbacks the recorded data at the same time using interrupt.
Hardware requirements
=====================
- Mini/micro USB cable
- TWR-KL28Z72M board
- Primary Elevator
- TWR-SGTL5000
- Headphone
- Line-in cable
- Personal Computer

Board settings
==============
The flexio_i2s_interrupt example is requires connecting the FLEXIO pins with the SAI pins so that
the I2S signals can route to the TWR-SGTL5000 Tower System module. Insert all boards into TWR-Elevator
The connection should be set as following:
- A76-Elevator, A22-Elevator connected
- A75-Elevator, A23-Elevator connected
- A79-Elevator, A25-Elevator connected
- A80-Elevator, A24-Elevator connected
- J7-SGTL5000, headphone connected
- J9-SGTL5000, line-in cable connected with PC


Prepare the Demo
================
1.  Connect a mini USB cable between the PC host and the OpenSDA USB port on the board.
2.  Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the demo runs successfully, the log would be seen on the OpenSDA terminal like as below,
You can hear a piece of sound which is a sin wave

~~~~~~~~~~~~~~~~~~~~~
FLEXIO I2S DMA example started!

FLEXIO I2S DMA example finished!
~~~~~~~~~~~~~~~~~~~~~

Toolchain supported
===================
- IAR embedded Workbench  8.32.1
- Keil MDK  5.26
- GCC ARM Embedded  7.3.1
- MCUXpresso 10.3.0

