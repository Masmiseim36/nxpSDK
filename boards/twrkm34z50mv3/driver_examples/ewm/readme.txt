Overview
========
The EWM Example project is to demonstrate usage of the KSDK EWM driver.
In the example, EWM counter is continuously refreshed until button is pressed.
Once the button is pressed, EWM counter will expire and interrupt will be generated.
After the first pressing, another interrupt can be triggered by pressing button again.

Note: In flash boot mode, if you want to use the IDE reset program, please use systemReset.

Toolchain supported
===================
- IAR embedded Workbench  9.10.2
- Keil MDK  5.34
- GCC ARM Embedded  10.2.1
- MCUXpresso  11.4.0

Hardware requirements
=====================
- TWR-KM34Z50MV3 board
- Mini USB cable
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
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

Running the demo
================

When the demo runs successfully, the following message is displayed in the terminal:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Press anykey to start the example...

EWM example
Press SW1 to expire EWM
EWM interrupt is occurred
Press SW1 to expire EWM again
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
