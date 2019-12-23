Overview
========
The FTM project is a demonstration program of the SDK FTM driver's output compare feature.
It sets up one FTM channel to toggle the output when a match occurs with the channel value. The user
should probe the FTM output with a oscilloscope to see the signal toggling.

Toolchain supported
===================
- IAR embedded Workbench 7.80.4
- Keil MDK 5.21a
- GCC ARM Embedded 2016-5.4-q3
- Kinetis Development Studio IDE 3.2.0
- MCUXpresso0.8

Hardware requirements
=====================
- Mini/micro USB cable
- TWR-KV10Z32 board
- Personal Computer
- Oscilloscope

Board settings
==============
No special settings are required.

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
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
FTM example for output compare

You will see the output signal toggle
Probe the signal using an oscilloscope
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

You can use oscilloscope to measure FTM signal output J15-9
Customization options
=====================

