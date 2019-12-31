Overview
========
The EWM Example project is to demonstrate usage of the KSDK EWM driver.
In the example, EWM counter is continuously refreshed until button is pressed.
Once the button is pressed, EWM counter will expire and interrupt will be generated.
After the first pressing, another interrupt can be triggered by pressing button again.

Toolchain supported
===================
- IAR embedded Workbench 7.80.4
- Keil MDK 5.21a
- GCC ARM Embedded 2016-5.4-q3
- Kinetis Development Studio IDE 3.2.0
- MCUXpresso0.8

Hardware requirements
=====================
- USB-KW24D512 board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
1.  Connect the host PC and the USB port on the target board.
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

~~~~~~~~~~~~~~~~~~~~~
EWM example
Press SW1 to expire EWM
EWM interrupt is occurred
Press SW1 to expire EWM again
~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

