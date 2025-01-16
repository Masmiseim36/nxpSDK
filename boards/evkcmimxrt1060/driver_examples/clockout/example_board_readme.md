Hardware requirements
=====================
- Mini/micro USB cable
- EVK_MIMXRT1060 board
- Personal Computer

Board settings
============
Connect pin:
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
To measure the output signal of CLKO1, please use the oscilloscope to probe TP15;
To measure the output signal of CLKO2, please use the oscilloscope to probe TP12.
These instructions are displayed/shown on the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Clock Output Driver Example.
Please press any key to continue.
Please Choose one clock to be outputted on CLKO1.
	A -- USB1 PLL Clock(Divided by 2).
	B -- SYS PLL Clock(Divided by 2).
	C -- VIDEO PLL Clock(Divided by 2).
	D -- Semc Clock Root.
	E -- Lcdif Pix Clock Root.
	F -- AHB Clock Root.
	G -- IPG Clock Root.
	H -- Perclk Root.
	I -- Ckil Sync Clock Root.
	J -- PLL4 Main Clock.
a

Please set the clock divider, range from 1 to 8.
2

Theoretically, the output frequency of CLKO1 is 120000000 Hz.
Please Choose one clock to be outputted on CLKO2.
	A -- Usdhc1 Clock Root.
	B -- LPI2C Clock Root.
	C -- CSI Clock Root.
	D -- OSC Clock.
	E -- Usdhc2 Clock Root.
	F -- SAI1 Clock Root.
	G -- SAI2 Clock Root.
t(Shared with ADC1 and ADC2 alt_clk root).
	I -- Can Clock Root(FlexCAN, shared with CANFD).
	J -- Flexspi Clock Root.
	K -- UART Clock Root.
	L -- Spdif0 Clock Root.
k

Please set the clock divider, range from 1 to 8.
2

Theoretically, the output frequency of CLKO2 is 40000000 Hz.

Please press any key to continue.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
