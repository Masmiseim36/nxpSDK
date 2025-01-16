Hardware requirements
=====================
- Micro USB cable
- JTAG/SWD debugger
- EVKB-MIMXRT1170 board
- Personal Computer
- Headphones with 3.5 mm stereo jack
- SD card

Board settings
==============
To make the examples work,
1. Please remove below resistors if on board wifi chip is not DNP:
    - R228
    - R229
    - R232
    - R234
2. Please make sure R136 is weld for GPIO card detect.

Prepare the Demo
================
1. Connect a micro USB cable between the PC host and the debug USB port (J86) on the board
2. Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3. Download the program to the target board.
4. Connect the headphones into the headphone jack on EVKB-MIMXRT1170 board (J101).
5. Either press the reset button on your board or launch the debugger in your IDE to begin
    running the demo.

# Known issues
============
1. After several tens of runs (the number of runs is not deterministic), the board restarts
   because a power-up sequence is detected on the RESET pin (due to a voltage drop).

