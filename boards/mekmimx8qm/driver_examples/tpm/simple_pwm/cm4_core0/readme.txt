Overview
========
The TPM project is a simple demonstration program of the SDK TPM driver. It sets up the TPM
hardware block to output a center-aligned PWM signal. The PWM dutycycle is manually updated.
On boards that have an LED connected to the TPM pins, the user will see a change in LED
brightness if user enter different values.

Toolchain supported
===================
- IAR embedded Workbench  8.32.1
- GCC ARM Embedded  7.3.1

Hardware requirements
=====================
- Micro USB cable
- i.MX8QM MEK Board
- MCIMX8-8X-BB
- J-Link Debug Probe
- 12V power supply
- Personal Computer
- Oscilloscope

Board settings
==============
- CM4_0: Connect J27-1(On base board) to Oscilloscope

Prepare the Demo
================
1.  Connect 12V power supply and J-Link Debug Probe to the board.
2.  Connect a USB cable between the host PC and the Debug port on the board (Refer "Getting Started with MCUXpresso SDK for i.MX 8QuadMax.pdf" for debug port information).
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board (Please refer "Getting Started with MCUXpresso SDK for i.MX 8QuadMax.pdf" for how to run different targets).
5.  Launch the debugger in your IDE to begin running the example.

Running the demo
================
When the example runs successfully, the following message is displayed in the terminal:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
TPM example to output center-aligned PWM signal

If an LED is connected to the TPM pin, you will see a change in LED brightness if you enter different values
If no LED is connected to the TPM pin, then probe the signal using an oscilloscope
Please enter a value to update the Duty cycle:
Note: The range of value is 0 to 9.
For example: If enter '5', the duty cycle will be set to 50 percent.
Value:5
The duty cycle was successfully updated!

Please enter a value to update the Duty cycle:
Note: The range of value is 0 to 9.
For example: If enter '5', the duty cycle will be set to 50 percent.
Value:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

