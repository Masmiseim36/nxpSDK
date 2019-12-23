Overview
========
The uart_edma ring buffer example shows how to use uart driver with EDMA:

In this example, one uart instance connect to PC through uart, the board will
send back all characters that PC send to the board. And data from PC will be firstly 
stored to a ring buffer using EDMA methord, and than application will read the ring buffer 
in a certain time, such as 10ms. While data has already be stored in the ring buffer,
routine will send them back.

And the ring buffer is achieved based on the DMA's source address modulo feature, the base address
should be aligned to a 0-modulo-boundary.

Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- Keil MDK  5.29
- GCC ARM Embedded  8.3.1
- MCUXpresso  11.1.0

Hardware requirements
=====================
- Mini/Micro USB cable
- TWR-KV46F150M board
- Personal Computer

Board settings
==============
To use UART on board, make sure that J505-3 connects to J505-4, J506-3 connects to J506-4

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
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the demo runs successfully, the log would be seen on the OpenSDA terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~
UART EDMA example
Send back received data
Echo every 8 characters

~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

