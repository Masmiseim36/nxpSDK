Overview
========

The adc_polling example shows the simplest way to use adc driver.

In this example, user should indicate a channel to provide a voltage signal (can be controlled by user) as the adc's
sample input. When running the project, typing any key into debug console would trigger the conversion. The execution 
would check the conversion completed flag in loop until the flag is asserted, which means the conversion is completed. 
Then read the conversion result value and print it to debug console.

Note, the default setting of initialization for the ADC converter is just an available configuration. User can change
the configuration structure's setting in application to fit the special requirement.

The auto-calibration is not essential but strongly recommended. It can help to adjust the converter itself and improve
the adc's performance.

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

ADC_5HC polling Example.
ADC_5HC_DoAutoCalibration() Done.
Press any key to get user channel's ADC_5HC value ...
ADC_5HC Value: 1918
Press any key to get user channel's ADC_5HC value.
ADC_5HC Value: 1635
Press any key to get user channel's ADC_5HC value.
ADC_5HC Value: 1412
Press any key to get user channel's ADC_5HC value.
ADC_5HC Value: 1332
Press any key to get user channel's ADC_5HC value.
ADC_5HC Value: 1543
Press any key to get user channel's ADC_5HC value.
ADC_5HC Value: 1579
Press any key to get user channel's ADC_5HC value.
...
Customization options
=====================

