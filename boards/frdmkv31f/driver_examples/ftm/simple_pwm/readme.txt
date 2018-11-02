Overview
========
The FTM project is a simple demonstration program of the SDK FTM driver. It sets up the FTM
hardware block to output a center-aligned PWM signal. The PWM dutycycle is periodically updated.
On boards that have an LED connected to the FTM pins, the user will see a change in LED brightness.

Toolchain supported
===================
- Keil MDK 5.24a
- IAR embedded Workbench 8.22.2
- GCC ARM Embedded 7-2017-q4-major
- MCUXpresso10.2.0

Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-KV31F board
- Personal Computer
- Oscilloscope

Board settings
==============
No special settings are required.

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

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
FTM example to output center-aligned PWM signal

You will see a change in LED brightness if an LED is connected to the FTM pin
If no LED is connected to the FTM pin, then probe the signal using an oscilloscope
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
- Green RGD will change brightness.
- You can use oscilloscope to measure FTM out(FTM0 channel 7 - TP17)
Customization options
=====================

