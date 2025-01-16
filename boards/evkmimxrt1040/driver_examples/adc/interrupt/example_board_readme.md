Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT1040-EVK board
- Personal Computer

Board settings
============
ADC1_IN9 is ADC input. Please sample voltage by J33-3 pin.

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

