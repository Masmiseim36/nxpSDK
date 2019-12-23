Overview
========
The Thermistor Lab Demo application demonstrates how to use PDB to trigger ADC and measure on-board thermistor.
This demo shows how to configure and use the ADC module to sample the differential voltage across onboard thermistors RT1-RT4.
If the user touches any on-board thermistor with a finger, the lab application detects a change in the thermistor temperature
and starts flashing the corresponding LED pair.

Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- Keil MDK  5.29
- GCC ARM Embedded  8.3.1
- MCUXpresso  11.1.0

Hardware requirements
=====================
- Mini/micro USB cable
- TWR-KV46F150M board
- Personal Computer

Board settings
==============
Default settings are required.
To use UART on board, make sure that J505-3 connects to J505-4, J506-3 connects to J506-4

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
The log below shows the output in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Thermistor Lab CADC demo.
Begin test...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Change the temperature of thermistors to see changes of LEDs.
Customization options
=====================

