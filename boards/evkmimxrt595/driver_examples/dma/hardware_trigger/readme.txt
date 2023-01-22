Overview
========
The DMA hardware trigger example is a simple demonstration program that
uses the SDK software.It executes one shot transfer from source buffer to
destination buffer using the SDK DMA drivers by hardware trigger.The purpose of 
this example is to show how to use the DMA and to provide a simple example 
fordebugging and further development.

Toolchain supported
===================
- IAR embedded Workbench  9.32.1
- Keil MDK  5.37
- GCC ARM Embedded  10.3.1
- MCUXpresso  11.7.0

Hardware requirements
=====================
- Micro USB cable
- EVK-MIMXRT595 board
- Personal Computer

Board settings
==============


Prepare the Demo
================
1.  Connect a micro USB cable between the PC host and the CMSIS DAP USB port (J40) on the board
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
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DMA hardware trigger example begin.

Destination Buffer:
0	0	0	0	

Press SW1 to trigger one shot DMA transfer.

SW1 is pressed.

DMA hardware trigger example finish.

Destination Buffer:
1	2	3	4	
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
