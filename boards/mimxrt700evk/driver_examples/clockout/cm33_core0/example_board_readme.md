Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT700-EVK Board
- Personal Computer

Board settings
============
No special settings are required.

Prepare the Demo
===============
1. Connect the micro and mini USB cable between the PC host and the USB ports on the board.
2. Open a serial terminal on PC for the serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running
   the demo.

Running the demo
================
The log below shows the output of the clockout demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Clock Output Driver Example.
Please choose one output clock.
	A -- Common VDD2 Base Clock.
	B -- MAIN PLL PFD0 Clock.
	C -- FRO0 Max Clock.
	D -- FRO1 Max Clock.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
To measure the output clock signal, please use the oscilloscope to probe J3-50.
