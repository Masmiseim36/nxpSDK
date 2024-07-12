Overview
========
The usart_dma example shows how to use usart driver with DMA:

In this example, one usart instance connect to PC through usart, the board will
send back all characters that PC send to the board.

Note: The example echo every 8 characters, so input 8 characters every time.

SDK version
===========
- Version: 2.16.000

Toolchain supported
===================
- IAR embedded Workbench  9.60.1
- Keil MDK  5.39.0
- GCC ARM Embedded  13.2.1
- MCUXpresso  11.10.0

Hardware requirements
=====================
- Micro USB cable
- EVK-MIMXRT595 board
- Personal Computer

Board settings
==============

Prepare the Demo
================
1.  Connect a micro USB cable between the PC host and the CMSIS DAP USB port (J40) on the board
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the demo runs successfully, the log would be seen on the terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
USART DMA example
Send back received data
Echo every 8 characters
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
