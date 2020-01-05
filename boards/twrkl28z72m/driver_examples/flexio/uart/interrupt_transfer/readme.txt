Overview
========
The flexio_uart_interrupt example shows how to use flexio uart driver in interrupt way:

In this example, a flexio simulated uart connect to PC through USB-Serial, the board will send back all characters
that PC send to the board. Note: two queued transfer in this example, so please input even number characters.
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
Flexio uart interrupt example
Board receives 8 characters then sends them out
Now please input:
~~~~~~~~~~~~~~~~~~~~~

Toolchain supported
===================
- IAR embedded Workbench  8.32.1
- Keil MDK  5.26
- GCC ARM Embedded  7.3.1
- MCUXpresso 10.3.0

