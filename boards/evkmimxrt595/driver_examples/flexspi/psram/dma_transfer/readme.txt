Overview
========
The flexspi_psram_dma_transfer example shows how to use flexspi driver with dma:

In this example, flexspi will send data and operate the external PSRAM connected with FLEXSPI. Some simple flash command will
be executed, such as Read Data and Write Data.
Example will write/read through the whole chip, using DMA transfer way.

Toolchain supported
===================
- IAR embedded Workbench  9.32.1
- Keil MDK  5.37
- GCC ARM Embedded  10.3.1
- MCUXpresso  11.7.0

Hardware requirements
=====================
- Micro USB cable
- MIMXRT595-EVK board
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
The log below shows the output of the hello world demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
FLEXSPI example started!
DMA Command Read/Write data succeed at all address range!
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
