Hardware requirements
=====================
- Micro USB cable
- MIMXRT700-EVK
- Personal Computer

Board settings
============
The MMC_SelectBusTiming in mmccard_init will fail, need to be commented out.


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

Running the demo
===============
When the demo runs successfully, the log would be seen on the terminal like:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MMCCARD interrupt example.



Card user partition size xxxx bytes



Working condition:



  Voltage: xxxx



  Timing mode: xxxx



  Bus width: xxxx



  Freq : xxxx



Read/Write the card continuously until encounter error....



Write/read one data block......

Compare the read/write content......

The read/write content is consistent.

Write/read multiple data blocks......

Compare the read/write content......

The read/write content is consistent.

Erase data groups......



Input 'q' to quit read/write/erase process.

Input other char to read/write/erase data blocks again.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
