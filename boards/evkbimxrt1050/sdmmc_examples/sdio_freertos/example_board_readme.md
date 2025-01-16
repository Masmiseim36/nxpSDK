Hardware requirements
===================
- Micro USB cable
- evkbimxrt1050 board
- Personal Computer
- SDIO card(such as wifi card, we use ATHEROS AR6233X to test)

Board settings
============
Insert the sdio card into card slot.
Note that sdcard connector is for micro sd,but sdio connector is normal one, so you need a connector converter to connect.

Prepare the Demo
===============
1.  Connect a micro USB cable between the PC host and the LPC-Link USB port (J8) on the board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Reset the SoC and run the project.

Running the demo
===============
When the demo runs successfully, the log would be seen on the terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

SDIO freertos example.

Card inserted.

Read function CIS, in direct way

Read function CIS, in extended way, non-block mode, non-word aligned size

Read function CIS, in extended way, block mode, non-word aligned size

The read content is consistent.

Input 'q' to quit card access task.

Input other char to access again.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

If sdio card interrupt is generated, then more log will be generated like:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

SDIO interrupt is received

SDIO Pending interrupt 0

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~