Hardware requirements
=====================
- Micro USB cable
- MIMXRT700-EVK board
- Personal Computer
- USB to Com Converter

Board settings
============
1. Remove Jumper JP22.
2. Connect pin:
- RX of USB2COM to JP13-4
- TX of USB2COM to JP13-5
- GND of USB2COM to JP13-1

Prepare the Demo
===============
Note: MCUXpresso IDE project default debug console is semihost
1. Connect a micro USB cable between the PC host and the DEBUG PORT on the board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 7 data bits
    - No parity
    - One stop bit
    - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the demo runs successfully, the log would be seen on the MCU-LINK terminal like:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Lpuart interrupt example with seven data bits
Board receives 8 characters then sends them out
Now please input:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~