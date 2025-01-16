Hardware requirements
=====================
- Mini/micro USB cable
- EVK-MIMXRT1020 board
- Personal Computer

Board settings
============
No special settings are required.

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board. 
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the example runs successfully, you can see the similar information from the terminal as below.
~~~~~~~~~~~~~~~~~~~~~
edma signle transfer interleave start

0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	

edma signle transfer interleave finished

1	0	2	0	3	0	4	0	5	0	6	0	7	0	8	0	

edma minor loop interleave start

0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	

edma minor loop interleave finished

1	2	0	4	5	0	7	8	0	10	11	0	13	14	0	0	
~~~~~~~~~~~~~~~~~~~~~


Note:
To debug in qspiflash, following steps are needed:
1. Select the flash target and compile.
2. Set the SW8: 1 off 2 off 3 on 4 off, then power on the board and connect USB cable to J23.
3. Start debugging in IDE.
   - Keil: Click "Download (F8)" to program the image to qspiflash first then clicking "Start/Stop Debug Session (Ctrl+F5)" to start debugging.
