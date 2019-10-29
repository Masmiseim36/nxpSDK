Overview
========
The lpuart_polling Example project is to demonstrate usage of the KSDK lpuart driver.
In the example, you can send characters to the console back and they will be printed out onto console
 instantly.
Note: Please input one character at a time. If you input too many characters each time, the receiver may overflow
because the low level UART uses simple polling way for receiving. If you want to try inputting many characters each time,
just define DEBUG_CONSOLE_TRANSFER_NON_BLOCKING in your project to use the advanced debug console utility.

Toolchain supported
===================
- IAR embedded Workbench  8.32.3
- Keil MDK  5.27
- GCC ARM Embedded  8.2.1
- MCUXpresso  11.0.1

Hardware requirements
=====================
- Mini USB cable
- FRDM-K32L2B board
- Personal Computer

Board settings
==============
No special is needed.

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
4.  Reset the SoC and run the project.

Running the demo
================
When the demo runs successfully, the log would be seen on the OpenSDA terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Lpuart polling example
Board will send back received characters
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

