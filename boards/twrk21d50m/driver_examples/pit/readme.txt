Overview
========
The PIT project is a simple demonstration program of the SDK PIT driver. It sets up the PIT
hardware block to trigger a periodic interrupt after every 1 second. When the PIT interrupt is triggered
a message a printed on the UART terminal and an LED is toggled on the board.

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

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Starting channel No.0 ...
 Channel No.0 interrupt is occured !
 Channel No.0 interrupt is occured !
 Channel No.0 interrupt is occured !
....................
....................
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
And you will find the LED ORANGE is taking turns to shine.
Customization options
=====================

