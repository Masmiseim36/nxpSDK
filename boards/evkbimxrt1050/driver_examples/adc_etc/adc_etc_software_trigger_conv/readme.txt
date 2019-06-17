Overview
========

The adc_etc_software_trigger_conv example shows how to use the ADC_ETC to generate a ADC trigger by software trigger.

In this example, the ADC is configured with hardware trigger. Once ADC gets the trigger from the ADC_ETC, the conversion goes,
then the ADC_ETC ISR would be executed.

Toolchain supported
===================
- IAR embedded Workbench  8.32.3
- Keil MDK  5.27
- GCC ARM Embedded  8.2.1
- MCUXpresso  11.0.0

Hardware requirements
=====================
- Mini/micro USB cable
- EVKB-IMXRT1050 board
- Personal Computer

Board settings
==============
ADC0_IN15 is ADC input. Please sample voltage by J23-1 pin.

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board. 
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
The log below shows the output of the hello world demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ADC_ETC_Software_Trigger_Conv Example Start!
ADC_DoAntoCalibration() Done.
ADC Full Range: XXXX
Press any key to get user channel's ADC value.
ADC conversion vaule is 16
Press any key to get user channel's ADC value.
ADC conversion vaule is 20
Press any key to get user channel's ADC value.
ADC conversion vaule is 35
Press any key to get user channel's ADC value.
ADC conversion vaule is 43
Press any key to get user channel's ADC value.
ADC conversion vaule is 4
Press any key to get user channel's ADC value.
ADC conversion vaule is 28
Press any key to get user channel's ADC value.
...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


Customization options
=====================

