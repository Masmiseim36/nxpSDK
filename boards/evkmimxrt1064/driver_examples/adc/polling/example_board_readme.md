Hardware requirements
=====================
- Mini/micro USB cable
- EVK-MIMXRT1064 board
- Personal Computer

Board settings
============
ADC1_IN0 is ADC input. Please sample voltage by J23-2 pin.

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
ADC polling Example.
ADC_DoAutoCalibration() Done.
Press any key to get user channel's ADC value.
ADC Value: 951
Press any key to get user channel's ADC value.
ADC Value: 1169
Press any key to get user channel's ADC value.
ADC Value: 1243
Press any key to get user channel's ADC value.
...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

