Overview
========

The dac_basic example shows how to use DAC module simply as the general DAC converter.

When the DAC's buffer feature is not enabled, the first item of the buffer is used as the DAC output data register.
The converter would always output the value of the first item. In this example, it gets the value from terminal,
outputs the DAC output voltage through DAC output pin.

Toolchain supported
===================
- IAR embedded Workbench 7.80.4
- Keil MDK 5.21a
- GCC ARM Embedded 2016-5.4-q3
- Kinetis Development Studio IDE 3.2.0
- MCUXpresso0.8

Hardware requirements
=====================
- Mini/Micro USB cable
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
5.  A multimeter may be used to measure the DAC output voltage.

Running the demo
================
When the example runs successfully, the following message is displayed in the terminal:

~~~~~~~~~~~~~~~~~~~~~~~
DAC basic Example.
Please input a value (0 - 4095) to output with DAC: 200
Input value is 200
DAC out: 200
~~~~~~~~~~~~~~~~~~~~~~~~
Then user can measure the DAC output pin to check responding voltage.
Customization options
=====================

