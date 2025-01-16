Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT1024-EVK board
- Personal Computer

Board settings
============
No special settings are required.

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
To measure the output signal of CLKO1, please use the oscilloscope to probe J19_20;
To measure the output signal of CLKO2, please use the oscilloscope to probe J19_18.
These instructions are displayed/shown on the terminal window:
~~~~~~~~~~~~~~~~~~~~~
Clock Output Driver Example.
Please Choose one clock to be outputted on CLKO1.
	A -- PLL3 SW Clock.
	B -- PLL2(divided by 2).
	C -- ENET PLL(divided by 2).
	D -- Semc Clock Root.
	E -- AHB Clock Root.
	F -- IPG Clock Root.
	G -- Perclk Root.
	H -- PLL4 Main Clock.
a

Please set the clock divider, range from 1 to 8.
3

Theoretically, the output frequency of CLKO1 is 80000000 Hz.
Please Choose one clock to be outputted on CLKO2.
	A -- Usdhc1 Clock Root.
	B -- IPI2C Clock Root.
	C -- OSC Clock.
	D -- LPSPI Clock Root.
	E -- Usdhc2 Clock Root.
	F -- SAI1 Clock Root.
	G -- SAI2 Clock Root.
	H -- SAI3 Clock Root.
	I -- Trace Clock Root.
	J -- Can Clock Root.
	K -- Flexspi Clock Root.
	L -- UART Clock Root.
	M -- Spdif0 Clock Root.
m

Please set the clock divider, range from 1 to 8.
2

Theoretically, the output frequency of CLKO2 is 750000 Hz.

Please press any key to continue.
~~~~~~~~~~~~~~~~~~~~~