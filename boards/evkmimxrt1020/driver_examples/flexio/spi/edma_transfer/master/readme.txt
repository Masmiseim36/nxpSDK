Overview
========
The flexio_spi_master_edma example shows how to use flexio spi master  driver in edma way:

In this example, a flexio simulated master connect to a flexio simulated spi slave .

Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- Keil MDK  5.29
- GCC ARM Embedded  8.3.1
- MCUXpresso  11.1.0

Hardware requirements
=====================

Board settings
==============

To make the example work, connections needed to be as follows:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        SLAVE           connect to      MASTER
Pin Name   Board Location     Pin Name    Board Location
 MOSI        J18-6                MOSI       J18-6
 MISO        J18-5                MISO       J18-5
 SCK         J18-3                SCK        J18-3
 PCS0        J18-4                PCS0       J18-4
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Prepare the Demo
================
1. Connect a mini USB cable between the PC host and the OpenSDA USB port on the board.
2. Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running
   the demo.

Running the demo
================
You can see the similar message shows following in the terminal if the example runs successfully.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~
FlexIO SPI edma example
Master Start...

Master runs successfully!
~~~~~~~~~~~~~~~~~~~~~~~~~~~~


Note:
To debug in qspiflash, following steps are needed:
1. Select the flash target and compile.
3. Set the SW8: 1 off 2 off 3 on 4 off, then power on the board and connect USB cable to J23.
4. Start debugging in IDE.
   - Keil: Click "Download (F8)" to program the image to qspiflash first then clicking "Start/Stop Debug Session (Ctrl+F5)" to start debugging.
Customization options
=====================

