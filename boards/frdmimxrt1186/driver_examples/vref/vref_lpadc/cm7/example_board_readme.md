Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-IMXRT1186 board
- Personal Computer

Board settings
============
Change Jumper J27 from 1-2 to 2-3
- Connect ADC CHA1 (J51-8)(GPIO_AD_14) to VREFO(J37-1).

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
 vref lpadc example

 vref default trim value is: 7
 vref default output voltage is 1194.595mv

 current setting trim value is 37
 current theoretical voltage is 1214.595mv
 current actual voltage is 1214.817mv
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Due to the errata051385, the ADC reference voltage is connect to the VDDA_ADC_1P8.
