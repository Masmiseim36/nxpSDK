Overview
========
The FTM project is a demonstration program of the SDK FTM driver's input capture feature.
The example sets up a FTM channel for dual-edge capture. Once the input signal is received,
this example will print the capture value.

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
- Personal Computer

Board settings
==============
Connect input signal to B23-Elevator

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
These instructions are displayed/shown on the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~
FTM input capture example

Once the input signal is received the input capture value is printed

Capture value C(n)V=baaa
~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

