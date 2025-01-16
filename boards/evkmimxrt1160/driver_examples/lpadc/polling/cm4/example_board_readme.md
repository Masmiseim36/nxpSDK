Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT1160-EVK board
- Personal Computer

Board settings
============
ADC1_IN0 is ADC input. Please sample voltage by J9-10 pin.

Prepare the Demo
===============
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
LPADC Polling Example
ADC Full Range: 4096
Full channel scale (Factor of 1).
Please press any key to get user channel's ADC value.
ADC Value: 951
ADC Value: 1169
ADC Value: 1243
...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

