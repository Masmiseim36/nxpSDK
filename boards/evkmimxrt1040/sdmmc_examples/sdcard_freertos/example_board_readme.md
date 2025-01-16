Hardware requirements
===================
- Mini/micro USB cable
- MIMXRT1040-EVK board
- Personal Computer
- SD card

Board settings
============
Please insert the SDCARD into card slot
Note: 
If you want to use a SD3.0 card with SD3.0 protocol, just insert a SD3.0 card into the card slot will be ok.
As the EVK board limitaion, there is eletronic mux added for switch between M.2 and sdcard which will affect the sdcard SDR104 timing.
So the maximum sd card timing frequency been decreased to 100MHZ to improve the stability.
User can remove the limitation by change the macro BOARD_SDMMC_SD_HOST_SUPPORT_SDR104_FREQ in sdmmc_config.h from (100000000U) to (200000000U).


Prepare the Demo
===============
1.  Connect a micro USB cable between the PC host and the OpenSDA USB port on the board.
2.  Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Insert SD card to card slot 
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
===============
When the demo runs successfully, the log would be seen on the terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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
