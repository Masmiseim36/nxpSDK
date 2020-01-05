Overview
========
The flexio_uart_interrupt example shows how to use flexio uart driver in interrupt way:

In this example, a flexio simulated uart connect to PC through USB-Serial, the board will send back all characters
that PC send to the board. Note: two queued transfer in this example, so please input even number characters.
Hardware requirements
=====================
- Mini/micro USB cable
- TWR-KL82Z72M board
- Primary Elevator
- USB-to-COM external
- Personal Computer

Board settings
==============
The example is requires connecting the FLEXIO pins with the USB2COM pins
Insert TWR-KL82Z72M board into Primary Elevator. The connection should be set as following:
- B70-Elevator, TX of USB2COM connected
- B69-Elevator, RX of USB2COM connected
- A5-Elevator, Ground of USB2COM connected

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
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
When the example runs successfully, the following message is displayed in the terminal:

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

