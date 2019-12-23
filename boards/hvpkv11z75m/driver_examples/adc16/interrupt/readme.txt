Overview
========

The adc16_interrupt example shows how to use interrupt with ADC16 driver.

In this example, user should indicate a channel to provide a voltage signal (can be controlled by user) as the ADC16's
sample input. When running the project, typing any key into debug console would trigger the conversion. ADC interrupt 
would be asserted once the conversion is completed. In ADC ISR, the conversion completed flag would be cleared by 
reading the conversion result value. Also, the conversion result value is stored, and the ISR counter is increased. 
These information would be printed when the execution return to the main loop.

The point is that the ADC16 interrupt configuration is set when configuring the ADC16's conversion channel. When in 
software trigger mode, the conversion would be launched by the operation of configuring channel, just like writing a
conversion command. So if user wants to generate the interrupt every time the conversion is completed, the channel's 
configuration with enabling interrupt setting would be used for each conversion.

Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- Keil MDK  5.29
- GCC ARM Embedded  8.3.1
- MCUXpresso  11.1.0

Hardware requirements
=====================
- Mini/Micro USB cable
- HVP-KV11Z75M board
- Personal Computer

Board settings
==============
No special settings are required.
It is used the internal temperature sensor (channel 26) on ADC0 for the example due to lack of the pins on HVP board itself.
Together with the main board other signals from it can be measured. Please select appropriate ADC channel in macro DEMO_ADC16_USER_CHANNEL.

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Connect external signal in J18-6
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the example runs successfully, the following message is displayed in the terminal:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ADC16 interrupt Example.
ADC16_DoAutoCalibration() Done.
ADC Full Range: XXXX
Press any key to get user channel's ADC value ...
ADC Value: 1918
ADC Interrupt Count: 1
ADC Value: 1635
ADC Interrupt Count: 2
ADC Value: 1412
ADC Interrupt Count: 3
ADC Value: 1332
ADC Interrupt Count: 4
ADC Value: 1543
ADC Interrupt Count: 5
ADC Value: 1579
ADC Interrupt Count: 6
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

