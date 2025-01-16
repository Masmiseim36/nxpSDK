Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT1180-EVK board
- Personal Computer

Board settings
============
1. Please check ADC chip(U45) on EVK board, if it is AMC1106M05 please connect TP41 to U45's pin 7.
2. Connect input voltage to J49(If ADC chip is AMC1106M05, intput voltage ranges from -50mV to +50mV; If ADC chip is
    AD7402, input voltage ranges from -200mV to +200mV).
3. If using the AMC1106M05, note that a differential input of 50 mV will produce a stream of ones and zeros that are
   high 89.06% of the time, not 100%. So result can be verified based on equation: SINC result =(ADCin / 64mV) * (128^3 )

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
The log below shows the output of the SINC ADC demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SINC ADC Example.

Press any key to trigger conversion!

Adc Result: 1193

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
