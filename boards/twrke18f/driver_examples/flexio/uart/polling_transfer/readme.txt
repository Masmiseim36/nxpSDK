Overview
========
The flexio_uart_polling example shows how to use flexio uart driver in polling way:

In this example, a flexio simulated uart connect to PC through USB-Serial, the board will send back
all characters that PC send to the board.

Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- Keil MDK  5.29
- GCC ARM Embedded  8.3.1
- MCUXpresso  11.1.0

Hardware requirements
=====================
- Micro USB cable
- TWR-KE18F board
- Primary Elevator
- USB to Com converter
- Personal Computer

Board settings
==============
The example requires connecting the FLEXIO pins with the USB2COM pins
The connection should be set as follows:
- B7-Elevator, TX of USB2COM connected
- B11-Elevator, RX of USB2COM connected
- B5-Elevator, Ground of USB2COM connected

Prepare the Demo
================
1. Connect a micro USB cable between the PC host and the OpenSDA USB port on the board.
2. Open a serial terminal on PC for OpenSDA serial device with these settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the demo runs successfully, the log would be seen on the UART Terminal port which connected to the USB2COM like:

~~~~~~~~~~~~~~~~~~~~~
Flexio uart polling example
Board will send back received characters
~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

