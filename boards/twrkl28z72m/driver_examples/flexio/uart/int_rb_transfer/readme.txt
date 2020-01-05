Overview
========
The flexio_uart_interrupt_ring_buffer example shows how to use flexio uart driver in interrupt way with
RX ring buffer enabled:

In this example, a flexio simulated uart connect to PC through USB-Serial, the board will
send back all characters that PC send to the board.

Note: The example echo every 8 characters, so input 8 characters every time.
Hardware requirements
=====================
- Mini/micro USB cable
- TWR-KL28Z72M board
- Personal Computer
- USB To Serial Adapter

Board settings
==============
Connect USB To Serial Adapter to user-defined pins, which are defined by "FLEXIO_UART_TX_PIN" and "FLEXIO_UART_RX_PIN" in source code.
    - PTE18, FLEXIO_UART_RX_PIN, Elevator-A9
    - PTE19, FLEXIO_UART_TX_PIN, Elevator-B21
    -        GND               , Elevator-B26

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
When the demo runs successfully, the log would be seen on the terminal(from USB To Serial Adapter) like:

~~~~~~~~~~~~~~~~~~~~~
FLEXIO UART RX ring buffer example
Send back received data
Echo every 8 bytes
~~~~~~~~~~~~~~~~~~~~~

Toolchain supported
===================
- IAR embedded Workbench  8.32.1
- Keil MDK  5.26
- GCC ARM Embedded  7.3.1
- MCUXpresso 10.3.0

