Overview
========
This lab shows how to configure and use the ADC module to sample the differential voltage across onboard thermistors RT1-RT4.
If the user touches any on-board thermistor with a finger, the lab application detects a change in the thermistor temperature
and starts flashing the corresponding LED pair.
• The lab tutorial demonstrates:
  – how to configure ADC module to read differential inputs
  – how to filter and process ADC results

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
- TWR-KV10Z32
- Personal Computer

Board settings
==============
- J11-1, J11-2 connected
- J11-3, J11-4 connected
- J12-1, J12-2 connected
- J12-3, J12-4 connected
- J13-1, J13-2 connected
- J13-3, J13-4 connected
- J14-1, J14-2 connected
- J14-3, J14-4 connected
- J8-2, J8-3 connected

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
When the demo runs successfully, the log would be seen on the terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Thermistor lab demo!
begin test...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Change the temperature of thermistors to see changes of LEDs.
Customization options
=====================

