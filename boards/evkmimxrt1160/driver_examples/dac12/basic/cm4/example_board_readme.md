Hardware requirements
=====================
- Micro USB cable
- MIMXRT1160-EVK board
- Personal Computer

Board settings
==============
No special settings are required.

**Please note this application can't support running with Linux BSP!**

Prepare the Demo
================
1. Connect 5V power supply and J-Link Debug Probe to the board, switch SW5 to power on the board.
2. Connect a micro USB cable between the host PC and the J11 USB port on the target board.
3. Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4. Download the program to the target board.
5. Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
When the demo runs successfully, following information can be seen on the terminal:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DAC basic Example.

Please input a value (0 - 4095) to output with DAC: 
Input value is 4095
DAC out: 4095
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Then user can measure the DAC0 output pin(TP18) to check responding voltage.
