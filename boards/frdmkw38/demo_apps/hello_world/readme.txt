Overview
========
The Hello World demo application provides a sanity check for the new SDK build environments and board bring up. The Hello
World demo prints the "Hello World" string to the terminal using the SDK UART drivers. The purpose of this demo is to
show how to use the UART, and to provide a simple project for debugging and further development.

Toolchain supported
===================
- IAR embedded Workbench  8.32.4
- MCUXpresso  11.1.0

Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-KW38 board
- Personal Computer

Board settings
==============
- Connect J14-1 (GND) to USB-serial GND
- Connect J14-5 (LPUART1_RX) to USB-serial TXD
- Connect J14-7 (LPUART1_TX) to USB-serial RXD

Prepare the Demo
================
1. Connect a USB cable between the PC host and the OpenSDA USB port on the board.
2. Open a serial terminal with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
These instructions are displayed/shown on the terminal window:
~~~~~~~~~~~~
hello world.
~~~~~~~~~~~~
Customization options
=====================

