Overview
========
The flexio_uart_polling example shows how to use flexio uart driver in polling way:

In this example, a flexio simulated uart connect to PC through USB-Serial, the board will send back
all characters that PC send to the board.
Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-KL28Z board
- Personal Computer
- USB To Serial Adapter

Board settings
==============
Connect USB To Serial Adapter to user-defined pins, which are defined by "FLEXIO_UART_TX_PIN" and "FLEXIO_UART_RX_PIN" in source code.
    - PTE18, FLEXIO_UART_RX_PIN, J2-15
    - PTE19, FLEXIO_UART_TX_PIN, J2-17
    -        GND               , J2-14

Prepare the Demo
================
1. Connect a mini USB cable between the PC host and the OpenSDA USB port on the board.
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
When the demo runs successfully, the log would be seen on the terminal(from USB To Serial Adapter) like:

~~~~~~~~~~~~~~~~~~~~~
Flexio uart polling example
Board will send back received characters
~~~~~~~~~~~~~~~~~~~~~

Toolchain supported
===================
- IAR embedded Workbench  8.32.1
- Keil MDK  5.26
- GCC ARM Embedded  7.3.1
- MCUXpresso 10.3.0

