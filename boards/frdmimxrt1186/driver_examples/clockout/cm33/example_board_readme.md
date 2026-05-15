Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-IMXRT1186 board
- Personal Computer

Board settings
============

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
To measure the output signal of CLKO1, please use the oscilloscope to probe TP57;
To measure the output signal of CLKO2, please use the oscilloscope to probe TP58.
These instructions are displayed/shown on the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Clock Output Driver Example.
Please Choose one clock to be outputted on CLKO1.
	1 -- OSC_RC_24M.
	2 -- OSC_RC_400M.
	3 -- PLL_480_DIV2.
	4 -- PLL_1G_DIV2.
1

Please set the clock divider, range from 1 to 256.
12

Theoretically, the output frequency of CLKO1 is 2000000 Hz.
Please Choose one clock to be outputted on CLKO2.
	1 -- OSC_RC_24M.
	2 -- OSC_RC_400M.
	3 -- PLL_1G_DIV5.
	4 -- PLL_ARM.
4

Please set the clock divider, range from 1 to 256.
10

Theoretically, the output frequency of CLKO2 is 79200000 Hz.

Please press any key to continue.


~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
