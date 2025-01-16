Hardware requirements
=====================
- Mini/micro USB cable
- EVK-MIMXRT1064 board
- Personal Computer

Board settings
============
No special settings are required.

Prepare the Demo
================
1. Connect a USB cable between the PC host and the OpenSDA USB port on the board.
2. Open a serial terminal with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
These instructions are displayed/shown on the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Starting channel No.0 ...
 Channel No.0 interrupt is occured !
 Channel No.0 interrupt is occured !
 Channel No.0 interrupt is occured !
....................
....................
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
And you will find the user LED is taking turns to shine.
