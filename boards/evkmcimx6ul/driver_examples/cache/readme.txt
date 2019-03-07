Overview
========

The cache example shows how to use memory cache driver.

In this example, many memory (such as SDRAM, etc) and DMA will be used to show the example.
Those memory is both accessible for cpu and DMA. For the memory data alignment, cache driver should be 
used rightly. For cache enabled case, the cache invalidate operation should be done before cpu access the memory. The cache clean opeartion should be done after CPU verify the memory
memory and before DMA access the memory.

Toolchain supported
===================
- IAR embedded Workbench 7.80.4
- GCC ARM Embedded 2016-5.4-q3

Hardware requirements
=====================
- Micro USB cable
- MCIMX6UL-EVK  board
- JLink Plus
- 5V power supply
- Personal Computer

Board settings
==============
No special is needed.

Prepare the Demo
================
1.  Connect 5V power supply and JLink Plus to the board, switch SW2001 to power on the board
2.  Connect a USB cable between the host PC and the J1901 USB port on the target board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the example runs successfully, following information can be seen on the terminal:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Cache example start.

 Cache Invalidate success! 

 Cache Clean success! 
 
 Cache example end. 

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

