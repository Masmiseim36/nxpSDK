Overview
========

The adc_etc_hardware_trigger_conv example shows how to use the ADC_ETC to generate a ADC trigger by PIT channel0 trigger.

Every 1 second, PIT channel0 would send a trigger signal to ADC_ETC, which can arbitrate and manage multiple external triggers,
and ADC_ETC would generate ADC trigger.

In this example, the ADC is configured with hardware trigger. Once ADC gets the trigger from the ADC_ETC, the conversion goes,
then the ADC_ETC ISR would be executed.
Hardware requirements
=====================
- Mini/micro USB cable
- EVKB-IMXRT1050 board
- Personal Computer

Board settings
==============
ADC0_IN15 and ADC0_IN0 are ADC inputs. Please sample voltage by J23-1 and J23-2 pins.

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
ADC_ETC_Hardware_Trigger_Conv Example Start!
ADC_DoAntoCalibration() Done.
ADC Full Range: XXXX
Please press any key to get user channel's ADC value.
ADC conversion vaule is 107 and 3882
ADC conversion vaule is 103 and 3884
ADC conversion vaule is 104 and 3880
ADC conversion vaule is 88 and 3890
ADC conversion vaule is 88 and 3890
ADC conversion vaule is 88 and 3890
ADC conversion vaule is 104 and 3882
ADC conversion vaule is 104 and 3882
...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


Toolchain supported
===================
- IAR embedded Workbench  8.32.1
- Keil MDK  5.26
- GCC ARM Embedded  7.3.1
- MCUXpresso 10.3.0

