Hardware requirements
=====================
- Micro USB cable
- MIMXRT700-EVK board
- Personal Computer

Board settings
============


Prepare the Demo
===============
1.  Connect a micro USB cable between the PC host and the CMSIS DAP USB port (J54) on the board
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
This example run both M33 and DSP at the same time!

When the demo runs successfully, the log would be seen on the M33's terminal like below
and the LED D1(BLUE LED) will be turned on/off according to the log.


~~~~~~~~~~~~~~~~~~~~~
Sema42 example!

Press any key to unlock semaphore and DSP core will turn off the LED
~~~~~~~~~~~~~~~~~~~~~

When press any key in the terminal, the terminal tells

~~~~~~~~~~~~~~~~~~~~~
Now the LED should be turned off

Sema42 example succeed!
~~~~~~~~~~~~~~~~~~~~~
