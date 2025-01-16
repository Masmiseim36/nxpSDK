Hardware requirements
=====================
- Micro USB cable
- EVK-MIMXRT595 board
- Personal Computer
- SDIO card

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

Note:
The Rev.B. EVK board can not perform Card power reset due to board limitation.
Please insert the SDIO card into card slot(J32) after the debug console print "Please insert a card into board."

Running the demo
===============
When the demo runs successfully, the log would be seen on the terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

SDIO card simple example.

Please insert a card into board.

Card inserted.

Read function CIS, in direct way

CIS read successfully
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Please note that below log maybe printed according to the card capability.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Read function CIS, in extended way, non-block mode, non-word aligned size

Read function CIS, in extended way, block mode, non-word aligned size

The read content is consistent.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
