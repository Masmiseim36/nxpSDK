Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT1060-EVKC board
- Personal Computer

Board settings
============
ADC0_IN15 is settings to ADC input, please sample voltage by J33-1 pin.

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
The log below shows the output of the adc etc edma demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Adc AutoCalibration Succeed.
 Adc Edma Transfer Example Start.
 Press any key to print output adc current convert value and voltage.
 ADC current convert value = 2016, voltage = 1.624
 Press any key to print output adc current convert value and voltage.
 ADC current convert value = 2023, voltage = 1.630
 Press any key to print output adc current convert value and voltage.
 ADC current convert value = 2017, voltage = 1.625
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~