Overview
========
The flexio_i2s_EDMA example shows how to use flexio_i2s driver with EDMA:

In this example, flexio acts as I2S module to record data from line-in line and playbacks the recorded data at the same time using EDMA.
Hardware requirements
=====================
- Mini/micro USB cable
- EVK-MIMXRT1020 board
- Headphone
- Personal Computer

Board settings
==============
- J11, headphone connected
- remove R85,R87,R88,R89 and connect FLEXIO pins to the pad close to U10 as follows:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Pin Name   Board Location     Pin Name    Board Location
RX_DATA    J18-4              RX_DATA     U10-16
TX_DATA    J18-3              TX_DATA     U10-14
SYNC       J18-2              SYNC        U10-13
BCLK       J18-1              BCLK        U10-12
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
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
When the demo runs successfully, the log would be seen on the OpenSDA terminal like as below.

~~~~~~~~~~~~~~~~~~~~~
FLEXIO I2S EDMA example started!

FLEXIO I2S EDMA example finished!
~~~~~~~~~~~~~~~~~~~~~

Note:
To debug in qspiflash, following steps are needed:
1. Select the flash target and compile.
3. Set the SW8: 1 off 2 off 3 on 4 off, then power on the board and connect USB cable to J23.
4. Start debugging in IDE.
   - Keil: Click "Download (F8)" to program the image to qspiflash first then clicking "Start/Stop Debug Session (Ctrl+F5)" to start debugging.

Toolchain supported
===================
- IAR embedded Workbench  8.32.1
- Keil MDK  5.26
- GCC ARM Embedded  7.3.1
- MCUXpresso 10.3.0

