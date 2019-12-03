Overview
========

The lpdc_single_interrupt example shows how to use interrupt with LPADC driver.

In this example, user should indicate a channel to provide a voltage signal (can be controlled by user) as the LPADC's
sample input. When running the project, typing any key into debug console would trigger the conversion. ADC watermark 
interrupt would be asserted once the number of datawords stored in the ADC Result FIFO is greater than watermark value.
In ADC ISR, the watermark flag would be cleared by reading the conversion result value. Also, result information would
be printed when the execution return to the main function.


Toolchain supported
===================
- IAR embedded Workbench  8.32.1
- GCC ARM Embedded  7.3.1

Hardware requirements
=====================
- Micro USB cable
- i.MX8QX MEK Board
- MCIMX8-8X-BB
- J-Link Debug Probe
- 12V power supply
- Personal Computer

Board settings
==============
- Input voltage signal(0~1.8V) to J13-B11 (LPADC0 CH4, on cpu board)

Prepare the Demo
================
1.  Connect 12V power supply and J-Link Debug Probe to the board.
2.  Connect a USB cable between the host PC and the Debug port on the board (Refer "Getting Started with MCUXpresso SDK for i.MX 8QuadXPlus.pdf" for debug port information).
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Launch the debugger in your IDE to begin running the example.

Running the demo
================
When the example runs successfully, the following message is displayed in the terminal:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
LPADC Interrupt Example
ADC Full Range: 4096
Full channel scale (Factor of 1).
Please press any key to get user channel's ADC value.
ADC value: 1334
ADC interrupt count: 1
ADC value: 1335
ADC interrupt count: 2
ADC value: 1335
ADC interrupt count: 3
...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

