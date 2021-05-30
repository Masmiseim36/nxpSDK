Overview
========
The EWM Example project is to demonstrate usage of the KSDK EWM driver.
In the example, EWM counter is continuously refreshed until button is pressed.
Once the button is pressed, EWM counter will expire and interrupt will be generated.
After the first pressing, another interrupt can be triggered by pressing button again.

Note: In flash boot mode, if you want to use the IDE reset program, please use systemReset.

Toolchain supported
===================
- IAR embedded Workbench  8.50.9
- Keil MDK  5.33
- GCC ARM Embedded  9.3.1
- MCUXpresso  11.3.0

Hardware requirements
=====================
- J-Link ARM
- FRDM-K66F board
- Mini/micro USB cable
- Personal Computer

Board settings
==============
No special is needed.

Prepare the Demo
================
1. Connect a USB cable between the PC host and the OpenSDA USB port on the board.
2. Open a serial terminal with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

Running the demo
================

When the example runs successfully, you can see the similar information from the terminal as below.

~~~~~~~~~~~~~~~~~~~~~
Press anykey to start the example...

EWM example
Press SW3 to expire EWM
EWM interrupt is occurred
Press SW3 to expire EWM again
~~~~~~~~~~~~~~~~~~~~~
