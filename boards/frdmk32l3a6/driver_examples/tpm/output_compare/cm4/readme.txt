Overview
========
The TPM project is a demonstration program of the SDK TPM driver's output compare feature.
It sets up one TPM channel to toggle the output when a match occurs with the channel value. The user
should probe the TPM output with a oscilloscope to see the signal toggling.

Toolchain supported
===================
- IAR embedded Workbench  8.32.3
- Keil MDK  5.27
- MCUXpresso  11.0.1
- GCC ARM Embedded  8.2.1

Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-K32L3A6 board
- Personal Computer
- Oscilloscope

Board settings
==============
No special settings are required.

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
TPM example for output compare

You will see the output signal toggle
Probe the signal using an oscilloscope
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
M4 core: Use oscilloscope to measure output signal pin at PTB2(J2-2) pin of FRDM-K32L3A6 board.
M0 core: Use oscilloscope to measure output signal pin at PTB13(J1-8) pin of FRDM-K32L3A6 board.
Customization options
=====================

