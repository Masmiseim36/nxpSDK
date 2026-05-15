Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-IMXRT1186 board
- Personal Computer

Board settings
============
Change Jumper J27 from 1-2 to 2-3
- ADC CHA1 input signal J51-8(GPIO_AD_14).

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
LPADC Polling Example
ADC Full Range: 65536
Full channel scale (Factor of 1).
Please press any key to get user channel's ADC value.
ADC value: 10538
ADC value: 10538
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
The analog voltage input range is theoretically 0-1.8V. However, due to the workaround for ERR051152, the maximum voltage should be limited to 1.78V.
Due to the errata051385, the ADC reference voltage is connect to the VDDA_ADC_1P8.
