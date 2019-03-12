Overview
========
The flexio_i2s_EDMA example shows how to use flexio_i2s driver with EDMA:

In this example, flexio acts as I2S module to record data from line-in line and playbacks the recorded data at the same time using EDMA.

Toolchain supported
===================
- GCC ARM Embedded  7.3.1
- IAR embedded Workbench  8.32.3
- Keil MDK  5.26
- MCUXpresso  10.3.1

Hardware requirements
=====================
- Mini/micro USB cable
- EVK-MIMXRT1015 board
- Headphone
- Personal Computer

Board settings
==============
- J11, headphone connected

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

Known issue:
If pressing SW9 to rerun the demo frequently, it may cause booting from QSPI nor flash failed, since the SA1 RXD0 pin is muxed as boot configurations pin.
Workaround: To use SW3 instead of SW9.

Note:
To debug in qspiflash, following steps are needed:
1. Select the flash target and compile.
3. Set the SW8: 1 off 2 off 3 on 4 off, then power on the board and connect USB cable to J41.
4. Start debugging in IDE.
   - Keil: Click "Download (F8)" to program the image to qspiflash first then clicking "Start/Stop Debug Session (Ctrl+F5)" to start debugging.
Customization options
=====================

