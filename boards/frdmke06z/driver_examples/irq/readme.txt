Overview
========
The IRQ Example project is a demonstration program that uses the MCUXpresso SDK software to use
IRQ pin interrupt.
The example uses the IRQ pin to generate a falling edge interrupt to show the example.

Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- Keil MDK  5.29
- GCC ARM Embedded  8.3.1
- MCUXpresso  11.1.0

Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-KE06Z board
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
Press the SW1 button to trigger IRQ interrupt, the log is show below:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 IRQ Driver Example Start.

 The IRQ interrupt has happened!

 IRQ Driver Example End.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

