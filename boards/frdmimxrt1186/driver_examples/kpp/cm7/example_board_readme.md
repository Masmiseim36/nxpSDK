Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-IMXRT1186 board
- Personal Computer

Board settings
============
Change Jumper J28, J31, J32, J35 from 1-2 to 2-3. 
Matrix and Jumper settings for KPP:

row5  <----->   J51 #4 (AD_16)
row4  <----->   J51 #12 (AD_18)
col5  <----->   J51 #6 (AD_17)
col4  <----->   J51 #10 (AD_19)

Prepare the Demo
===============
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
===============

When you press any key on matrix,  the log will show the right key you have pressed.
If the press is long, it will add the long press mention.
The log would be seen on the OpenSDA terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 KPP Driver Example Start.
  
 Key SW1 was pressed.

 Key SW3 was pressed.

 Key SW1 was released.

 Key SW3 was released.

 All keys released.

 This was a long press.

   ......

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
