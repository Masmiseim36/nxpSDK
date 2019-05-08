Overview
========
The qspi_polling example shows how to use qspi driver with polling:

In this example, qspi will send data and operate the external flash connected with QSPI. Some simple flash command will
be executed, such as Write Enable, Erase sector, Program page.
Example will first erase the sector and programe a page into the flash, at last check if the data in flash is correct.

Notice: While do program, it is not suggested to make the flash write frequency bigger than the core clock frequency.
In some tool chain's Debug version, this may cause core do not have enough speed to send data to flash.
The flash write frequency equals to QSPI working frequency plus data line number. For example, if QSPI working in 48MHz,
program command uses quad mode, the write frequency is 48MHz * 4 = 192MHz.
Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-KL82Z board
- Personal Computer

Board settings
==============
The example is uses QSPI0. The module uses PTE0~PTE11 pins.
No special jumper needs to be connected.

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
When the example runs successfully, the following message is displayed in the terminal:

~~~~~~~~~~~~~~~~~~~~~
QSPI example started!

Erase finished!

Program data finished!

Program through QSPI polling succeed!
~~~~~~~~~~~~~~~~~~~~~

Toolchain supported
===================
- IAR embedded Workbench  8.32.1
- Keil MDK  5.26
- GCC ARM Embedded  7.3.1
- MCUXpresso 10.3.0

