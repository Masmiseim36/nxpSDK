Overview
========
The FTM project is a demonstration program of the SDK FTM driver's output compare feature.
It sets up one FTM channel to toggle the output when a match occurs with the channel value. The user
should probe the FTM output with a oscilloscope to see the signal toggling.

Toolchain supported
===================
- IAR embedded Workbench  8.32.1
- GCC ARM Embedded  7.3.1

Hardware requirements
=====================
- Micro USB cable
- MIMX8QX MEK board
- J-Link Debug Probe
- 12V power supply
- Personal Computer
- Oscilloscope

Board settings
==============
Use oscilloscope to probe the signal on U2-4 (On base board).

#### Please note this application can't support running with Linux BSP! ####

Prepare the Demo
================
1.  Connect 12V power supply and J-Link Debug Probe to the board, switch SW3 to power on the board.
2.  Connect a micro USB cable between the host PC and the J11 USB port on the cpu board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Launch the debugger in your IDE to begin running the example.

Running the demo
================
These instructions are displayed/shown on the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~
FTM example for output compare
You will see the output signal toggle
Probe the signal using an oscilloscope
~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

