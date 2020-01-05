Overview
========

The local memory controller cache example shows how to use local memory cache driver.

In this example, the sdram memory will be used to show the example.
The sdram memory is both accessible for cpu and EDMA. For the memory data alignment, cache driver should be 
used rightly. The code cache driver and system cache driver can be used when the relevant cache mode is enabled.
In code cache enabled case, the code cache enabled memory mapped in sdram is write though/non-cacheable mode.
so the sdram memory invalidate should be done before cpu read the memory. In system cache enabled case, when the
sdram memory is write-back mode, the memory invalidate should be done before cpu read the memory and the
memory push should be done before EDMA access the memory.

Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- Keil MDK  5.29
- GCC ARM Embedded  8.3.1
- MCUXpresso  11.1.0

Hardware requirements
=====================
- Mini USB cable
- TWRK80F150M REVB board
- Personal Computer

Board settings
==============
no shunt on J6.
Note: Remove C11 on TWRK80F150M REVB board.

Prepare the Demo
================
1.  Connect a mini/micro USB cable between the PC host and the USB port on the board. 
2.  Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the demo runs successfully, the log would be seen on the OpenSDA terminal like:

 LMEM Controller Cache Example Start. 

 The data in cache is synchronized with the sdram memory.

 The memory for edma transfer is synchronized with sdram memory.

 LMEM controller example end.


Customization options
=====================

