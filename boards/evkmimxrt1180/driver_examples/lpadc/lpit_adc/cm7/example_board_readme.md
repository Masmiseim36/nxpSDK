Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT1180-EVK board
- Personal Computer

Board settings
============
- ADC CH7A input signal J41-2(GPIO_AD_02).

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
===============
When the demo runs successfully, the log would be seen on the OpenSDA terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
LPADC Interrupt Example
ADC Full Range: 4096
Full channel scale (Factor of 1).
ADC interrupt count: 1
ADC value: 572
ADC interrupt count: 2
ADC value: 575
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
The analog voltage input range is theoretically 0-1.8V. However, due to the workaround for ERR051152, the maximum voltage should be limited to 1.78V.
Due to the errata051385, the ADC reference voltage is connect to the VDDA_ADC_1P8.
