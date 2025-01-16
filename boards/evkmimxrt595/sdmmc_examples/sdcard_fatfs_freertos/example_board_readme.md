Hardware requirements
=====================
- Micro USB cable
- EVK-MIMXRT595 board
- Personal Computer
- SD Card

Board settings
============
Make sure resistors R691~R697 are populated and resistors R611~R620,R660, R661 are removed.

Prepare the Demo
===============
1.  Connect a micro USB cable between the PC host and the CMSIS DAP USB port (J40) on the board
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Launch the debugger in your IDE to begin running the demo.

Please insert the SDCARD into card slot(J32)

Running the demo
===============
When the demo runs successfully, the log would be seen on the terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SDCARD fatfs freertos example.

Card inserted.

Make file system......The time may be long if the card capacity is big.

Create directory......
TASK1: write file successed.
TASK2: write file successed.
TASK1: finsied.
TASK2: finsied.


​~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~