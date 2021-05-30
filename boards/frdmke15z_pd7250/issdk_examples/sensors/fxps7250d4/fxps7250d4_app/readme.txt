Hardware requirements
=====================
- Micro USB cable
- FRDM-KE15Z board
- FRDM-STBA-PD7250 shield board
- Personal Computer

Board settings
==============
Overview
========
The FXPS7250D4 Poll example application demonstrates the use of the FXPS7250D4 sensor in poll Mode using I2C.
The example demonstrates configuration of all registers reguired to put the sensor into polling mode to keep
checking sensor status and read out a raw pressure, pressure in kPa & temperature sample.

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
