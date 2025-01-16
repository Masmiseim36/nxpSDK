Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT1180-EVK board
- Personal Computer
- Two SPDIF RCA lines
- Soundcard support SPDIF interface

Board settings
============
1.Make sure J23,J25,J26,T1,T2,J24 and J27 are installed. Weld 10kΩ resistor to R375.
2.Remove R140/R141/R2158/R2159.

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Connect J27 with SPDIF soundcard output inteface, connect J24 with SPDIF soundcard input interface.
3.  Playback music using the SPDIF soundcard, make the SPDIF signals input to MIMXRT1180 board.
4.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
5.  Download the program to the target board.
6.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
After run the demo, you can hear the music playbacked from SPDIF soundcacrd, the log below shows the output of the example in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SPDIF EDMA example started!
SPDIF EDMA example finished!
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
