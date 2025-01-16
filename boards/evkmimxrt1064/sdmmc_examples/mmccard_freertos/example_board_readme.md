Hardware requirements
===================
- Mini/micro USB cable
- EVK-MIMXRT1064 board
- Personal Computer
- SD to Micro SD coverter
- MMC card


Board settings
============
Please insert the MMCCARD into card slot(J39)

Prepare the Demo
===============
1.  Connect a micro USB cable between the PC host and the OpenSDA USB port on the board.
2.  Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Insert MMC card to card slot
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
===============
When the demo runs successfully, the log would be seen on the terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MMCCARD freertos example.

Read/Write/Erase the card continuously until encounter error......

Write/read one data block......
Compare the read/write content......
The read/write content is consistent.
Write/read multiple data blocks......
Compare the read/write content......
The read/write content is consistent.
Erase multiple data blocks......

Input 'q' to quit read/write/erase process.
Input other char to read/write/erase data blocks again.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
