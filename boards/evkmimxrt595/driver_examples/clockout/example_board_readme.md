Hardware requirements
=====================
- Micro USB cable
- EVK-MIMXRT595 board
- Personal Computer

Board settings
============
No special settings are required.

Prepare the Demo
===============
1.  Connect a micro USB cable between the PC host and the CMSIS DAP USB port (J40)  on the board
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Launch the debugger in your IDE to begin running the demo.

Running the demo
================
The log below shows the output of the clockout demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Clock Output Driver Example.
Please choose one output clock.
	A -- OSC_CLK Clock.
	B -- Low Power Oscillator Clock.
	C -- FRO_DIV2 Clock.
	D -- Main Clock.
	E -- Dsp Main Clock.
	F -- Main System PLL.
	G -- SYSPLL0 AUX0_PLL_Clock.
	H -- DSP PLL Clock.
	I -- SYSPLL0 AUX1_PLL_Clock.
	J -- AUDIO PLL Clock.
	K -- 32 KHz RTC Clock.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
To measure the output clock signal, please use the oscilloscope to probe JP27_2.
