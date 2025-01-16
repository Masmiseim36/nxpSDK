Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT1060-EVKC board
- Personal Computer
- Oscilloscope

Board settings
==============
weld resister 0Ω at R350 R356 R362.

* Probe the pwm signal using an oscilloscope
 - At J17-3
 - At J17-4
 - At J17-6
 - At TP15

Prepare the Demo
================
1. Connect a USB cable between the PC host and the OpenSDA USB port on the board.
2. Open a serial terminal with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
When the example runs successfully, the following message is displayed in the terminal:
~~~~~~~~~~~~~~~~~~~~~~~~
FlexPWM driver example
~~~~~~~~~~~~~~~~~~~~~~~
