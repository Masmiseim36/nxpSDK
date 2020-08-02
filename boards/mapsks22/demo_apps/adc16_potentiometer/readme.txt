Overview
========
The ADC potentiometer demo application demonstrates how to read the analog value from a hardware potentiometer via the
ADC peripheral.

Toolchain supported
===================
- Keil MDK 5.24a
- IAR embedded Workbench 8.22.2
- GCC ARM Embedded 7-2017-q4-major
- MCUXpresso10.2.0

Hardware requirements
=====================
- Mini/micro USB cable
- MAPS-KS22F256 board & MAPS-DOCK board
- Personal Computer

Board settings
==============
The adc_potentiometer demo is configured to use UART1 with PTE0 and PTE1 pins. To make the JLink serial device work,
the jumpers on MAPS-DOCK board should be set as following:

- JP7 on MAPS-Dock: UART0 related jumpers connected

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
When the demo runs successfully, the log would be seen on the OpenSDA terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ADC16_DoAutoCalibration() Done.

The ADC16 output value is 216.
The ADC16 output value is 215.
The ADC16 output value is 215.
.............................
.............................
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

