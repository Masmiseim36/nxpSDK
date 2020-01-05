Overview
========
The PWT project is a simple demonstration program of the SDK PWT driver. It sets up the PWT
hardware block to edge detection, capture control part and detects measurement trigger edges and
controls when and which pulse width register(s) will be updated. Once the input signal is received,
this example will print overflow, positive pulse width and negative pulse width.

Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- Keil MDK  5.29
- GCC ARM Embedded  8.3.1
- MCUXpresso  11.1.0

Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-KE06Z board
- Personal Computer

Board settings
==============
Connect input signal to J1-6

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
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the example runs successfully, the following message is displayed in the terminal:

~~~~~~~~~~~~

PWT example: input signal whose pulse width needs to be measured

PWT overflow occurs

PWT overflow occurs

Positive pulse width=33 usec

Negative pulse width=8 usec

Positive pulse width=33 usec

Negative pulse width=8 usec

~~~~~~~~~~~~
Customization options
=====================

