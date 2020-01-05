Overview
========

The adc12_interrupt example shows how to use interrupt with ADC12 driver.

In this example, user should indicate a channel to provide a voltage signal (can be controlled by user) as the ADC12's sample input.
When running the project, typing any key into debug console would trigger the conversion. ADC interrupt would be asserted once the conversion is completed. In ADC ISR, the conversion completed flag would be cleared by reading the conversion result value.
Also, the conversion result value is stored, and the ISR counter is increased. These information would be printed when the execution return to the main loop.

The point is that the ADC12 interrupt configuration is set when configuring the ADC12's conversion channel. When in software trigger mode, the conversion would be launched by the operation of configuring channel, just like writing a conversion command. So if user wants to generate the interrupt every time the conversion is completed, the channel's configuration with enabling interrupt setting would be used for each conversion.


Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- Keil MDK  5.29
- GCC ARM Embedded  8.3.1
- MCUXpresso  11.1.0

Hardware requirements
=====================
- Micro USB cable
- HVP-KE18F board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
1. Connect a micro USB cable between the PC host and the OpenSDA USB port on the board.
2. Open a serial terminal on PC for OpenSDA serial device with these settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the example runs successfully, following information can be seen on the OpenSDA terminal:
ADC12 interrupt Example.
ADC Full Range: 256
Press any key to get user channel's ADC value ...
Customization options
=====================

