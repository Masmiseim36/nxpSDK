Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT1180-EVK board
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
When the example runs successfully, the following message is displayed in the terminal:

~~~~~~~~~~~~~~~~~~~~~
----- Fast test starts -----
Low Byte test starts
Waiting for timeout reset
Low Byte test succeeded
----- High Byte test starts -----
Waiting for timeout reset
High Byte test succeeded
----- The end of RTWDOG fast test -----

----- Refresh test start -----
----- None-window mode -----
Refresh rtwdog 1 time
Refresh rtwdog 2 time
Refresh rtwdog 3 time
Refresh rtwdog 4 time
Refresh rtwdog 5 time
Refresh rtwdog 6 time
Waiting for time out reset
None-window mode reset succeeded
----- Window mode -----
Refresh rtwdog 1 time
Refresh rtwdog 2 time
Refresh rtwdog 3 time
Waiting for time out reset
Window mode reset succeeded
~~~~~~~~~~~~~~~~~~~~~

Note
===============
Reset event will happen when running this demo, so container header is needed. Please add container header by spsdk.