Overview
========

The QTMR project is a demonstration program of the SDK QTMR driver's input capture and output pwm feature.
The example sets up a QTMR channel for input capture. Once the input signal is received,
this example will enter interrupt and print the capture value.
The example also sets up one QTMR channel to output pwm. The user should probe a 50Khz PWM signal output with
50% dutycycle with a oscilloscope. 
Hardware requirements
=====================
- Mini/micro USB cable
- EVKB-IMXRT1050 board
- Personal Computer
- Oscilloscope

Board settings
==============
- Connect input signal to J23-6.
- Use oscilloscope to measure output the signal pin at J23-5.

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
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

****Input capture example.****

****Provide a signal input to the QTMR pin****

Captured Period time = 998 us (±5%)

****Output PWM example.****

*********Make sure to connect an oscilloscope.*********

****A 50 duty cycle PWM wave is observed on an oscilloscope.****

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


Toolchain supported
===================
- IAR embedded Workbench  8.32.1
- Keil MDK  5.26
- GCC ARM Embedded  7.3.1
- MCUXpresso 10.3.0

