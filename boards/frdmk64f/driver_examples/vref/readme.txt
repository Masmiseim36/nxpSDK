Overview
========
The vref example shows how to use the vref driver.

In this example, the adc16 module is initiealized by using VALT reference voltage, that is, VREF module provides a
stable reference voltage to ADC16.

Then, user should indicate a channel to provide a voltage signal (can be controlled by user) as the ADC16's sample
input. When running the project, typing any key into debug console would trigger the conversion. The execution would
check the conversion completed flag in loop until the flag is asserted, which means the conversion is completed. Then
read the conversion result value and print it to debug console.
Hardware requirements
=====================
- Mini/Micro USB cable
- FRDM-K64F board
- Personal Computer

Board settings
==============
- ADC CH12 input signal J4-2(ptb2).

Prepare the Demo
================
1.  Connect a USB cable between the PC host and the OpenSDA USB port on the board.
2.  Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Connect external signal source to user-defined channel, defined by "DEMO_ADC16_USER_CHANNEL" in source code. 
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the demo runs successfully, the log would be seen on the OpenSDA terminal like:
VREF example

ADC16_DoAutoCalibration() Done.

ADC Full Range: 4096

Defalut Trim Value: 34

Press any key to get user channel's ADC value ...

ADC value: 3724
ADC value: 3348
ADC value: 4033
ADC value: 4093
ADC value: 3692
ADC value: 3724
ADC value: 3348
ADC value: 3684



Toolchain supported
===================
- IAR embedded Workbench  8.32.1
- Keil MDK  5.26
- GCC ARM Embedded  7.3.1
- MCUXpresso 10.3.0

