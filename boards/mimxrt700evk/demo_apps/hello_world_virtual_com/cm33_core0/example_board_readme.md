Hardware requirements
=====================
- Mini/micro USB cable
- mimxrt700evk board
- Personal Computer

Board settings
============
No special settings are required.

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the MCU-LINK USB port on the target board. 
2.  Connect a USB cable between the host PC and the USB OTG1 port on the target board.
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.
5.  Open a serial terminal(like putty) to communicate with the board via a USB serial device (VID:PID = 1FC9:0094)
    running at the baud rate 115200 8n1.

Running the demo
================   
The log below shows the output of this demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
hello world.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Press any key. Terminal will echo back the received characters.
