Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT1160-EVK board
- Personal Computer

Board settings
============
ADC0_IN0 is ADC input. Please sample voltage by J9-10 pin.

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
The log below shows the output of the hello world demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ADC_ETC_Software_Trigger_Conv Example Start!
Press any key to get user channel's ADC value.
ADC conversion value is 16
Press any key to get user channel's ADC value.
ADC conversion value is 20
Press any key to get user channel's ADC value.
ADC conversion value is 35
Press any key to get user channel's ADC value.
ADC conversion value is 43
Press any key to get user channel's ADC value.
ADC conversion value is 4
Press any key to get user channel's ADC value.
ADC conversion value is 28
Press any key to get user channel's ADC value.
...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


