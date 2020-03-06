Overview
========
The usart_dma_low_power example shows how to use usart to wake up the system
in low power modes, and how to wake up for DMA only.
In this example, one usart instance connects to PC through usart, the board will
start receiving characters from PC and then go into low power mode. Each character
from PC will only wake up the DMA but the CPU keeps in low power mode. Once 8
characters were received, system will be woken up and echo the characters to PC.

Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- Keil MDK  5.29
- GCC ARM Embedded  8.3.1
- MCUXpresso  11.1.1

Hardware requirements
=====================
- Micro USB cable
- EVK-MIMXRT685 board
- Personal Computer

Board settings
==============

Prepare the Demo
================
1.  Connect a micro USB cable between the PC host and the CMSIS DAP USB port (J5) on the board
2.  Open a serial terminal with the following settings:
    - 9600 baud rate
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
USART DMA low power example
Send back received data
Echo every 8 characters (1st char 'q' leads to end).
12345678asdfghjkqwertyui
Done!
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

