Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT1160-EVK board
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
3.  Download CM4 image firstly, then download CM7 image.(Important Step)
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================   
After CM4 and CM7 image downloaded, the log below shows the output of the hello world demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Prepare to wake up core1 booted from flash, console will be transferred to it.
hello world.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
