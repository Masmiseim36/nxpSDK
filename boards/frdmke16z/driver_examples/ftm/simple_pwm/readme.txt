Overview
========
The FTM project is a simple demonstration program of the SDK FTM driver. It sets up the FTM
hardware block to output a center-aligned PWM signal. The PWM dutycycle is periodically updated.
On boards that have an LED connected to the FTM pins, the user will see a change in LED brightness.
Hardware requirements
=====================
- Micro USB cable
- FRDM-KE16Z board
- An oscilloscope
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
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the example runs successfully, the following message is displayed in the terminal:

~~~~~~~~~~~~~~~~~~~~~~~
FTM example to output center-aligned PWM signal

You will see a change in LED brightness if an LED is connected to the FTM pin

If no LED is connected to the FTM pin, then probe the signal using an oscilloscope
~~~~~~~~~~~~~~~~~~~~~~~
- Red led will change brightness.

Toolchain supported
===================
- IAR embedded Workbench  8.32.1
- Keil MDK  5.26
- GCC ARM Embedded  7.3.1
- MCUXpresso 10.2.1

