Overview
========
The KPP Example project is a demonstration program that uses the KSDK software to manipulate the Keypad MATRIX.
The example is use the continuous column and rows as 4*4 or 8*8 matrix to show the example.

Toolchain supported
===================
- IAR embedded Workbench  9.32.1
- Keil MDK  5.37
- MCUXpresso  11.7.0
- GCC ARM Embedded  10.3.1

Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT1170-EVK board
- Personal Computer

Board settings
==============
Remove 0Ω resistors R404,R406,R408.

Matrix and Jumper settings for KPP:

row2  <----->   J10 #8
row3  <----->   J10 #12
col2  <----->   J10 #10
col3  <----->   J10 #6

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Connect the 2*2 matrix to the Jumpers mentioned above. 
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================

When you press any key on matrix,  the log will show the right key you have pressed.
If the press is long, it will add the long press mention.
The log would be seen on the OpenSDA terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 KPP Driver Example Start.
  
 Key SW1 was pressed.

 Key SW3 was pressed.

 This was a long press.

   ......

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
