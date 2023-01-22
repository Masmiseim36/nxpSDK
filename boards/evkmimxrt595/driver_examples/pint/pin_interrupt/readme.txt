Overview
========
This example shows how to use SDK drivers to use the Pin interrupt & pattern match peripheral.

Toolchain supported
===================
- IAR embedded Workbench  9.32.1
- Keil MDK  5.37
- GCC ARM Embedded  10.3.1
- MCUXpresso  11.7.0

Hardware requirements
=====================
- Micro USB cable
- EVK-MIMXRT595 board
- Personal Computer

Board settings
==============

Prepare the Demo
================
1.  Connect a micro USB cable between the PC host and the CMSIS DAP USB port (J40) on the board
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Launch the debugger in your IDE to begin running the demo.

Running the demo
================
1. The following lines are printed to the serial terminal when the demo program is executed.

PINT Pin interrupt example

PINT Pin Interrupt events are configured

Press corresponding switches to generate events

2. This example configures "Pin Interrupt 0" to be invoked when SW1 switch is pressed by the user.
   The interrupt callback prints "PINT Pin Interrupt 0 event detected". "Pin Interrupt 1" is
   is configured to be invoked when  SW2 is pressed. The interrupt callback prints "PINT Pin Interrupt 
   1 event detected". "Pin Interrupt 2" is configured to be invoked when J30-1 pin has falling edge. The interrupt 
   callback prints "PINT Pin Interrupt 2 event detected".
