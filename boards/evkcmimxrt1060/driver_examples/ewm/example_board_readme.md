Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT1060-EVKC board
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
Press anykey to start the example...

EWM example
Press SW5 to expire EWM
EWM interrupt is occurred
Press SW5 to expire EWM again
~~~~~~~~~~~~~~~~~~~~~

Note:
This case will run to HardFault_Handler on IAR flexspi_nor_debug/flexspi_nor_release target. This issue will be fixed in later release.
