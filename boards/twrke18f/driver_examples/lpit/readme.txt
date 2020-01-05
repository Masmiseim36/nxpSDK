Overview
========
The LPIT project is a simple example of the SDK LPIT driver. It sets up the LPIT
hardware block to trigger a periodic interrupt after every 1 second. When the LPIT interrupt is triggered
a message a printed on the serial terminal and an LED is toggled on the board.


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
No need special setting

Prepare the Demo
================
1. Connect a micro USB cable between the PC host and the OpenSDA USB port on the board.
2. Open a serial terminal on PC for OpenSDA serial device with these settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the example runs successfully, you can see the similar information from the terminal as below.

~~~~~~~~~~~~~~~~~~~~~
Starting channel No.0 ...
 Channel No.0 interrupt is occurred !
 Channel No.0 interrupt is occurred !
 ........................
~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

