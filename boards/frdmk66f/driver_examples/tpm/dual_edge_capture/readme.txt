Overview
========
The TPM project is a demonstration program of the SDK TPM driver's dual-edge capture feature.
This feature is available only on certain SoC's.
The example sets up a TPM channel-pair for dual-edge capture. Once the input signal is received,
this example will print the capture values and period of the input signal on the terminal window.

Toolchain supported
===================
- IAR embedded Workbench  8.50.9
- Keil MDK  5.33
- GCC ARM Embedded  9.3.1
- MCUXpresso  11.3.0

Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-K66F board
- Personal Computer

Board settings
==============
- Connect input signal to PTB18(J2-2).

Prepare the Demo
================
1. Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2. Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
When the example runs successfully, you can see information from the terminal as below.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
TPM dual-edge capture example

Once the input signal is received the input capture values are printed

The input signal's pulse width is calculated from the capture values & printed

Capture value C(n)V=0

Capture value C(n+1)V=26d4

Input signals pulse width=82 us
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
