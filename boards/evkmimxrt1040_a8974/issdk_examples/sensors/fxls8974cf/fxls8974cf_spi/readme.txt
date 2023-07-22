Hardware requirements
===================
- Mini/micro USB cable
- MIMXRT1040-EVK board
- FRDM-STBI-A8974 board
- Personal Computer

Board settings
============
Populate pins 3,4,5 and 6 on J17 Arduino header on MIMXRT1040-EVK.
Since the examples use SPI, Pins 1-2 of SW2 on FRDM-STBI-A8974 should be connected.
SW1 Pins 2-3 should be connected to select default operating mode i.e. "ACCEL NORMAL" mode.

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
===============
When the demo runs successfully, you can see the Samples printed to the terminal. 

Note: You must rotate/drop the board to get the samples to change.
