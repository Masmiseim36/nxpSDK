Overview
========

The dac_basic example shows how to use DAC module simply as the general DAC converter.

When the DAC's buffer feature is not enabled, the first item of the buffer is used as the DAC output data register.
The converter would always output the value of the first item. In this example, it gets the value from terminal,
outputs the DAC output voltage through DAC output pin.

Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- Keil MDK  5.29
- GCC ARM Embedded  8.3.1
- MCUXpresso  11.1.0

Hardware requirements
=====================
- Mini/Micro USB cable
- TWR-KV46F150M board
- Personal Computer
- Primary Elevator
Board settings
==============
To use UART on board, make sure that J505-3 connects to J505-4, J506-3 connects to J506-4

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
When the example runs successfully, the following message is displayed in the terminal:

~~~~~~~~~~~~~~~~~~
DAC basic Example.
Please input a value (0 - 4095) to output with DAC: 200
Input value is 200
DAC out: 200
~~~~~~~~~~~~~~~~~~~

Then user can measure the DAC output pin (A32 - Primary Elevator ) to check responding voltage.
Customization options
=====================

