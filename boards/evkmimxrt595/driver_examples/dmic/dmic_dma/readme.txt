Overview
========
This example shows how to use DMA to transfer data from DMIC to memory.

Toolchain supported
===================
- IAR embedded Workbench  9.32.1
- Keil MDK  5.37
- GCC ARM Embedded  10.3.1
- MCUXpresso  11.7.0

Hardware requirements
=====================
- Micro USB cable
- EVK-MIMXRT595 board
- Personal Computer

Board settings
==============


Prepare the Demo
================
1.  Connect a micro USB cable between the PC host and the CMSIS DAP USB port on the board
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
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Configure DMA
Buffer Data before transfer
 <data>
Transfer completed
Buffer Data after transfer
 <data>

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
This example shows how DMA can be used with DMIC to transfer data to memory.DMIC audio data can also be seen in g_rxBuffer.
Initially this buffer is initialized to zero.
