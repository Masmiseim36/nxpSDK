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
- IAR embedded Workbench  8.40.2
- Keil MDK  5.29
- GCC ARM Embedded  8.3.1
- MCUXpresso  11.1.0

Hardware requirements
=====================
- Mini/micro USB cable
- TWR-KV11Z75M
- Personal Computer

Board settings
==============
- J6-1, J6-2 connected
- J6-3, J6-4 connected
- J7-1, J7-2 connected
- J7-3, J7-4 connected
- J8-1, J8-2 connected
- J8-3, J8-4 connected
- J9-1, J9-2 connected
- J9-3, J9-4 connected
- J13-2, J13-3 connected

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

