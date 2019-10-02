Overview
========
The FTM project is a simple demonstration program of the SDK FTM driver to use FTM as a timer.
It sets up the FTM hardware block to trigger an interrupt every 1 millisecond.
When the FTM interrupt is triggered a message a printed on the UART terminal.

Toolchain supported
===================
- IAR embedded Workbench  8.32.3
- Keil MDK  5.27
- GCC ARM Embedded  8.2.1
- MCUXpresso  11.0.0

Hardware requirements
=====================
- Mini/micro USB cable
- TWR-KV46F150M board
- Personal Computer

Board settings
==============
To use UART on board, make sure that J505-3 connects to J505-4, J506-3 connects to J506-4

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
~~~~~~~~~~~~~~~~~~~~~~~~
FTM example to simulate a timer

You will see a "-" or "|" in terminal every 1 second:
-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

