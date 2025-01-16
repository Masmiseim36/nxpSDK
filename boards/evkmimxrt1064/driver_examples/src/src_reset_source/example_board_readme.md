Hardware requirements
=====================
- Mini/micro USB cable
- EVK-MIMXRT1064 board
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
===============
When the demo runs successfully, the log would be seen on the terminal like:

Example: SRC Reset Soruce.
SRC_GetResetStatusFlags(): 0x3.
SRC_GetBootModeWord1(): 0x0.
SRC_GetBootModeWord2(): 0x2000009.

Note:
	SRC_GetBootModeWord2() values are different for different fuses and platforms.
