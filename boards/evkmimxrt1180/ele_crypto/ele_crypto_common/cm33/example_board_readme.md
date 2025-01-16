Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT1180-EVK board
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
Example output on terminal:

EdgeLock Enclave Sub-System crypto example:

****************** Load EdgeLock FW ***********************
EdgeLock FW loaded and authenticated successfully.

****************** Compute Hash (SHA256) of massage *******
*SUCCESS* Computed HASH matches the expected value.

*SUCCESS* HASH Init done.

*SUCCESS* HASH Update done.

*SUCCESS* Computed HASH (Finish) matches the expected value.

****************** Start RNG ******************************
EdgeLock RNG Start success.

EdgeLock RNG ready to use.

****************** Get RNG Random **********************
Get RNG random data successfully. First word: 0xbcd7f0de

End of Example with SUCCESS!!