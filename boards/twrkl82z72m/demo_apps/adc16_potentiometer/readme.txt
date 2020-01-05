Overview
========
The ADC potentiometer demo application demonstrates how to read the analog value from a hardware potentiometer via the
ADC peripheral.
Hardware requirements
=====================
- Mini/micro USB cable
- TWR-KL82Z72M board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the board.
2.  Open a serial terminal with following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running
    the demo.

Running the demo
================
Roll the potentiometer to change the ADC value.
When the example runs successfully, the following message is displayed in the terminal:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ADC16_DoAutoCalibration() Done.

The ADC16 output value is 216.
The ADC16 output value is 215.
The ADC16 output value is 215.
.............................
.............................
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Toolchain supported
===================
- IAR embedded Workbench  8.32.1
- Keil MDK  5.26
- GCC ARM Embedded  7.3.1
- MCUXpresso 10.3.0

