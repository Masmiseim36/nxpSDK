Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT1180-EVK board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
1. Connect a USB cable between the PC host and the OpenSDA USB on the board.
2. Open a serial terminal on PC for OpenSDA serial device with these settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running
   the example.

Running the demo
================
When the example runs successfully, following information can be seen on the OpenSDA terminal:

~~~~~~~~~~~~~~~~~~~~~~

==FlexCAN loopback functional example -- Start.==

Send message from MB8 to MB9
tx word0 = 0x11223344
tx word1 = 0x55667788

Received message from MB9
rx word0 = 0x11223344
rx word1 = 0x55667788

==FlexCAN loopback functional example -- Finish.==
~~~~~~~~~~~~~~~~~~~~~~
