Overview
========

The adc_interrupt example shows how to use interrupt with adc driver.

In this example, user should indicate a channel to provide a voltage signal (can be controlled by user) as the adc's
sample input. When running the project, typing any key into debug console would trigger the conversion. ADC interrupt 
would be asserted once the conversion is completed. In ADC ISR, the conversion completed flag would be cleared by 
reading the conversion result value. Also, the conversion result value is stored, and the ISR counter is increased. 
These information would be printed when the execution return to the main loop.

The point is that the adc interrupt configuration is set when configuring the adc's conversion channel. When in 
software trigger mode, the conversion would be launched by the operation of configuring channel, just like writing a
conversion command. So if user wants to generate the interrupt every time the conversion is completed, the channel's 
configuration with enabling interrupt setting would be used for each conversion.

Toolchain supported
===================
- IAR embedded Workbench 7.80.4
- GCC ARM Embedded 2016-5.4-q3

Hardware requirements
=====================
- Micro USB cable
- MCIMX6UL-EVK  board
- JLink Plus
- 5V power supply
- Personal Computer

Board settings
==============
Both sides of R1717 should be short-circuited.
The user can sample voltage by J1706-1 pin(ADC_IN0).

Prepare the Demo
================
1.  Connect 5V power supply and JLink Plus to the board, switch SW2001 to power on the board.
2.  Connect a USB cable between the host PC and the J1901 USB port on the target board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Connect external signal source to user-defined channel, defined by "DEMO_ADC_5HC_USER_CHANNEL" in source code. 
5.  Download the program to the target board.
6.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the demo runs successfully, the log would be seen on the terminal like:

ADC_5HC interrupt Example.
ADC_5HC_DoAutoCalibration() Done.
Press any key to get user channel's ADC_5HC value ...
ADC_5HC Value: 1918
ADC_5HC Interrupt Counter: 1
Press any key to get user channel's ADC_5HC value.
ADC_5HC Value: 1635
ADC_5HC Interrupt Counter: 2
Press any key to get user channel's ADC_5HC value.
ADC_5HC Value: 1412
ADC_5HC Interrupt Counter: 3
Press any key to get user channel's ADC_5HC value.
ADC_5HC Value: 1332
ADC_5HC Interrupt Counter: 4
Press any key to get user channel's ADC_5HC value.
ADC_5HC Value: 1543
ADC_5HC Interrupt Counter: 5
Press any key to get user channel's ADC_5HC value.
ADC_5HC Value: 1579
ADC_5HC Interrupt Counter: 6
Press any key to get user channel's ADC_5HC value.
...
Customization options
=====================

