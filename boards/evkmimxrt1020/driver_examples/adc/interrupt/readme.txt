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
- IAR embedded Workbench  8.40.2
- Keil MDK  5.29
- GCC ARM Embedded  8.3.1
- MCUXpresso  11.1.0

Hardware requirements
=====================
- Mini/micro USB cable
- EVK-MIMXRT1020 board
- Personal Computer

Board settings
==============
ADC1_IN0 is ADC input. Please sample voltage by J19-4 pin.

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
These instructions are displayed/shown on the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ADC interrupt Example.
ADC_DoAutoCalibration() Done.
ADC Full Range: XXXX
Press any key to get user channel's ADC value.
ADC Value: 1211
ADC Interrupt Counter: 1
Press any key to get user channel's ADC value.
ADC Value: 968
ADC Interrupt Counter: 2
Press any key to get user channel's ADC value.
ADC Value: 1161
ADC Interrupt Counter: 3
Press any key to get user channel's ADC value.
ADC Value: 422
ADC Interrupt Counter: 4
Press any key to get user channel's ADC value.
...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


Note:
To debug in qspiflash, following steps are needed:
1. Select the flash target and compile.
3. Set the SW8: 1 off 2 off 3 on 4 off, then power on the board and connect USB cable to J23.
4. Start debugging in IDE.
   - Keil: Click "Download (F8)" to program the image to qspiflash first then clicking "Start/Stop Debug Session (Ctrl+F5)" to start debugging.
Customization options
=====================

