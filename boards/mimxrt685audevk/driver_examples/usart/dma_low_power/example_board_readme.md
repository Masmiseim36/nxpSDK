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
    - 9600 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the demo runs successfully, the starting logs of the USART dma low power example should be displayed like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
USART DMA low power example
Send back received data
Echo every 8 characters (1st char 'q' leads to end).
------------------
Input any key to start.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Then after sending one random character, the following log on the terminal should be like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Now enter deep sleep and wait for 8 characters.
qwer1234
Done!
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
