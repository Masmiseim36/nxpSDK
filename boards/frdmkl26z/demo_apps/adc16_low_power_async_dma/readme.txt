Overview
========
The ADC Low Power Async DMA demo application demonstrates the usage of the ADC and DMA peripheral while in a low power mode. The
microcontroller is first set to very low power stop (VLPS) mode. Every 100 ms, low power timer trigger the ADC module convert
value on ADC channel. After 16 times(1,6s) the DMA transfer finish interrupt wake up the CPU to process sampled data, print result to
user and toggle LED.

Toolchain supported
===================
- IAR embedded Workbench 7.80.4
- Keil MDK 5.21a
- GCC ARM Embedded 2016-5.4-q3
- Kinetis Development Studio IDE 3.2.0
- MCUXpresso0.8

Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-KL26Z board
- Personal Computer

Board settings
==============
This example project does not call for any special hardware configurations.
Although not required, the recommendation is to leave the development board's jumper settings
and configurations in default state when running this example.

Prepare the Demo
================
1.  Connect a USB cable between the PC host and the OpenSDA USB port on the board.
2.  Open a serial terminal with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running
    the demo.

Running the demo
================
When the demo runs successfully, the log would be seen on the OpenSDA terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ADC LOW POWER ASYNC DMA DEMO
ADC value: 15670
ADC value: 16634
ADC value: 16686
ADC value: 16681

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Note:
 - LED will be toggled when the CPU wake up.
Customization options
=====================

