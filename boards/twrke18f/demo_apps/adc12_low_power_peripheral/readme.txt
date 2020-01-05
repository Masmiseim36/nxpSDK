Overview
========
The ADC Low Power Demo project is a demonstration program that uses the KSDK software. The
microcontroller is set to a very low power stop (VLPS) mode, and every 500 ms an interrupt wakes up
the ADC module and takes the current temperature sensor value of the microcontroller. While the 
temperature remains within boundaries, both LEDs are on. If the temperature is higher or lower than
average, a led comes off. This demo provides an example to show how ADC works during a VLPS mode and
a simple debugging.

Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- Keil MDK  5.29
- GCC ARM Embedded  8.3.1
- MCUXpresso  11.1.0

Hardware requirements
=====================
- Micro USB cable
- TWR-KE18F board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
1.  Connect a micro USB cable between the host PC and the OpenSDA USB port on the target board.
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
When the demo runs successfully, the following message is displayed in the terminal:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ADC LOW POWER DEMO
 The Low Power ADC project is designed to work with the Kinetis SDK.
 1. Set your target board in a place where the temperature is constant.
 2. Wait until two Led light turns on.
 3. Increment or decrement the temperature to see the changes.
 Wait two led on...

 Enter any character to begin...

 ---> OK! Main process is running...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Note:
 - when the temperature is above the average: LED RED on, LED GREEN off.
 - when the temperature is below the average: LED GREEN on, LED RED off.
Customization options
=====================

