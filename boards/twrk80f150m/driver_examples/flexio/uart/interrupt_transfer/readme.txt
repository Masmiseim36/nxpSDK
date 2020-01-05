Overview
========
The flexio_uart_interrupt example shows how to use flexio uart driver in interrupt way:

In this example, a flexio simulated uart connect to PC through USB-Serial, the board will send back all characters
that PC send to the board. Note: two queued transfer in this example, so please input even number characters.

Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- Keil MDK  5.29
- GCC ARM Embedded  8.3.1
- MCUXpresso  11.1.0

Hardware requirements
=====================
- Mini/micro USB cable
- TWR-K80F150M board
- Primary Elevator
- USB-to-COM external
- Personal Computer

Board settings
==============
The flexio_uart_interrupt example is requires connecting the FLEXIO pins with the USB2COM pins
Insert TWR-K80F150M board into Primary Elevator. The connection should be set as following:
- B70-Elevator, TX of USB2COM connected
- B69-Elevator, RX of USB2COM connected
- A5-Elevator, Ground of USB2COM connected

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
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the demo runs successfully, the log would be seen on the OpenSDA terminal like:

~~~~~~~~~~~~~~~~~~~~~
Flexio uart interrupt example
Board receives 8 characters then sends them out
Now please input:
~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

