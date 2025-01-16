Hardware requirements
=====================
- Micro USB cable
- MIMXRT700-EVK board
- Personal Computer
- SDIO card

Board settings
============
JP65 1-2 connected.

Prepare the Demo
===============
1.  Connect a micro USB cable between the PC host and the MCU-LINK USB port on the board
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Launch the debugger in your IDE to begin running the demo.

Note:
Please insert the SDIO card into card slot(J47) after the debug console print "Please insert a card into board."

Running the demo
===============
When the demo runs successfully, the log would be seen on the terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SDIO freertos example.

Card inserted.

Card init finished, ready to access.

Read function CIS, in direct way

CIS read successfully

Read function CIS, in extended way, non-block mode, non-word aligned size

Read function CIS, in extended way, block mode, non-word aligned size

The read content is consistent.

Input 'q' to quit card access task.            
Input other char to access again
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
