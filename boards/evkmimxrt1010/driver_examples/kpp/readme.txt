Overview
========
The KPP Example project is a demonstration program that uses the KSDK software to manipulate the Keypad MATRIX.
The example is use the continuous column and rows as 4*4 or 8*8 matrix to show the example.

Toolchain supported
===================
- IAR embedded Workbench  8.32.3
- Keil MDK  5.27
- GCC ARM Embedded  8.2.1
- MCUXpresso  11.0.1

Hardware requirements
=====================
- Mini/micro USB cable
- EVK-MIMXRT1010 board
- Personal Computer

Board settings
==============
Matrix and Jumper settings for KPP:

row1  <----->   J57-6
row2  <----->   J57-8
row3  <----->   J57-18
col1  <----->   J57-12
col2  <----->   J57-10
col3  <----->   J57-20

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Connect the 3*3 matrix to the Jumpers mentioned above. 
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================

When you press any key on matrix,  the log will show the right key you have pressed.
If you press a key continuously, it will print "this was a long press" repeately.
The log would be seen on the OpenSDA terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 KPP Driver Example Start.
  
 Key SW1 was pressed.

 Key SW3 was pressed.

 This was a long press.
 This was a long press.
   ......
   
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Note:
To debug in qspiflash, following steps are needed:
1. Select the flash target and compile.
2. Set the SW8: 1 off 2 off 3 on 4 off, then power on the board and connect USB cable to J41.
3. Start debugging in IDE.
   - Keil: Click "Download (F8)" to program the image to qspiflash first then clicking "Start/Stop Debug Session (Ctrl+F5)" to start debugging.
Customization options
=====================

