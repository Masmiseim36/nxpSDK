Hardware requirements
=====================
- Micro USB cable
- EVK-MIMXRT685 board
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
When the example runs successfully, the following message is displayed in the terminal:
~~~~~~~~~~~~~~~~~~~~~
DMA channel chain example begin.
Destination Buffer:
0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0
DMA channel chain example finish.
Destination Buffer:
1	2	3	4	11	22	33	44 111 222 333 444	1111 2222 3333 4444
~~~~~~~~~~~~~~~~~~~~~

