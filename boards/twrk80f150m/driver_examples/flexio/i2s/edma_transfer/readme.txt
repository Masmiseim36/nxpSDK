Overview
========
The flexio_i2s_EDMA example shows how to use flexio_i2s driver with EDMA:

In this example, flexio acts as I2S module to record data from line-in line and playbacks the recorded data at the same time using EDMA.

Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- Keil MDK  5.29
- GCC ARM Embedded  8.3.1
- MCUXpresso  11.1.0

Hardware requirements
=====================
- Mini/micro USB cable
- TWR-K80F150M board
- Primary Elevator
- TWR-SGTL5000
- Headphone
- Personal Computer

Board settings
==============
The example is requires connecting the FLEXIO pins with the SAI pins so that
the I2S signals can route to the TWR-SGTL5000 Tower System module. Insert all boards into TWR-Elevator
The connection should be set as following:
- B70-Elevator, A22-Elevator connected
- B69-Elevator, A23-Elevator connected
- A37-Elevator, A25-Elevator connected
- A38-Elevator, A24-Elevator connected
- J7-SGTL5000, headphone connected
- J9-SGTL5000, line-in line connected with PC

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
4.  Playback a music in your PC.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
When the demo runs successfully, the log would be seen on the OpenSDA terminal like as below,
You can hear the music played in the PC from the headphone inserted in SGTL-5000 board.

~~~~~~~~~~~~~~~~~~~~~
FLEXIO I2S EDMA example started!

FLEXIO I2S EDMA example finished!
~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

