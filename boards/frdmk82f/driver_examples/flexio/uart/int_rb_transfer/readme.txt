Overview
========
The flexio_uart_interrupt_ring_buffer example shows how to use flexio uart driver in interrupt way with
RX ring buffer enabled:

In this example, a flexio simulated uart connect to PC through USB-Serial, the board will
send back all characters that PC send to the board.

Note: The example echo every 8 characters, so input 8 characters every time.

Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- Keil MDK  5.29
- GCC ARM Embedded  8.3.1
- MCUXpresso  11.1.0

Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-K82F board
- USB-to-COM external
- Personal Computer

Board settings
==============
The flexio_uart_interrupt_ring_buffer example is requires connecting the FLEXIO pins with the USB2COM pins
The connection should be set as following:
- J12-16, TX of USB2COM connected
- J12-15, RX of USB2COM connected
- J12-2, Ground of USB2COM connected

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
When the demo runs successfully, the log would be seen on the UART Terminal port which connected to the USB2COM like:

~~~~~~~~~~~~~~~~~~~~~
FLEXIO UART RX ring buffer example
Send back received data
Echo every 8 bytes
~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

