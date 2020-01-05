Hardware requirements
=====================
- Micro USB cable
- FRDM-KE15Z board
- FRDM-STBI-DP300x shield board
- Personal Computer

Board settings
==============
Overview
========
The NPS300x Interrupt example application demonstrates the use of the NPS300x sensor in poll (Non-Buffered) Mode.
The example demonstrates configuration of all registers reguired to put the sensor
into Standard Mode and read out a Pressure+Temperature sample.

The sensor reads samples as per the configured Auto acquisition time step and sets a Flag.
The application consistently checks the Flag and when set reads out the sample.

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
${ANCHOR}
