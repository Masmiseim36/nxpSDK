Overview
========
The GPIO Example project is a demonstration program that uses the KSDK software to manipulate the general-purpose
outputs.
The example is supported by the set, clear, and toggle write-only registers for each port output data register. The 
example uses the software button to control/toggle the LED.

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
- FRDM-KL26Z board
- Personal Computer

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

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
GPIO Driver example
Press SW1 to turn on/off a LED
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
If you press the SW1, the LED RED will be toggled, and "SW1 is pressed" is shown on the terminal window.
Customization options
=====================

