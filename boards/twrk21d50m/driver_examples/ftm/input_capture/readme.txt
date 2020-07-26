Overview
========
The FTM project is a demonstration program of the SDK FTM driver's input capture feature.
The example sets up a FTM channel for dual-edge capture. Once the input signal is received,
this example will print the capture value.

Toolchain supported
===================
- IAR embedded Workbench 8.11.3
- Keil MDK 5.23
- GCC ARM Embedded 6-2017-q2
- Kinetis Development Studio IDE 3.2.0
- MCUXpresso10.1.0

Hardware requirements
=====================
- Mini/micro USB cable
- TWR-K21D50M board
- Personal Computer
- Elevator tower

Board settings
==============
Connect input to A40 elevator tower.

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

~~~~~~~~~~~~~~~~~~~~~~~
FTM input capture example
Once the input signal is received the input capture value is printed
Capture value C(n)V=7402
~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

