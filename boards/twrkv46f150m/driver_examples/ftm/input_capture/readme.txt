Overview
========
The FTM project is a demonstration program of the SDK FTM driver's input capture feature.
The example sets up a FTM channel for dual-edge capture. Once the input signal is received,
this example will print the capture value.
Hardware requirements
=====================
- Mini/micro USB cable
- TWR-KV46F150M board
- Personal Computer
- Primary Elevator Board

Board settings
==============
To use UART on board, make sure that J505-3 connects to J505-4, J506-3 connects to J506-4
Connect input into A34

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
- These results are displayed/shown on the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~
FTM input capture example

Once the input signal is received the input capture value is printed

Capture value C(n)V=32c(*)
~~~~~~~~~~~~~~~~~~~~~~~
(*) This value maybe difference, depend on input signal.

Toolchain supported
===================
- IAR embedded Workbench  8.32.1
- Keil MDK  5.26
- GCC ARM Embedded  7.3.1
- MCUXpresso 10.3.0

