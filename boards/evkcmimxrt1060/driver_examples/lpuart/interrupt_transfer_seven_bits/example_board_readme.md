Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT1060-EVKC board
- Personal Computer
- USB to Com Converter

Board settings
============
Connect pin:
- RX of USB2COM to J16-2
- TX of USB2COM to J16-1
- GND of USB2COM to J17-7

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 7 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the example runs successfully, the following message is displayed in the terminal(from USB To Serial Adapter):

~~~~~~~~~~~~~~~~~~~~~
Lpuart interrupt example with seven data bits
Board receives 8 characters then sends them out
Now please input:
~~~~~~~~~~~~~~~~~~~~~
