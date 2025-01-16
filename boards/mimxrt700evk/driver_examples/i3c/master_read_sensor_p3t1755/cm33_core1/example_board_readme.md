Hardware requirements
=====================
- Micro USB cable
- MIMXRT700-EVK board
- Personal Computer

Board settings
============
To make the example work with cm33_core0, connections needed to be as follows:
Make sure R581, R586, R589 are soldered and R582, R585, R588 are removed.

Prepare the Demo
===============
1.  Connect a USB Type-C cable between the host PC and the MCU-Link port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Press reset button to begin running the demo.

Running the demo
================
The log below shows the output of the hello world demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
I3C master read sensor data example.

Temperature:27.000000

Temperature:27.000000

......

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
