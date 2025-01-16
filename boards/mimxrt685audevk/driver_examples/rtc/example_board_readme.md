Hardware requirements
=====================
- Micro USB cable
- MIMXRT685-AUD-EVK board
- Personal Computer

Board settings
============


Prepare the Demo
===============
1.  Connect a micro USB cable between the PC host and the CMSIS DAP USB port (J5) on the board
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
The log below shows example output of the RTC driver demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
RTC example: set up time to wake up an alarm
Current datetime: 2014-12-25 19: 0: 0
Please input the number of second to wait for alarm
The second must be positive value
Alarm will occur at: 2014-12-25 19: 0: 5
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
