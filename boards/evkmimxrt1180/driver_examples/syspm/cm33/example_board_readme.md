Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT1180-EVK board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
1. Connect a micro USB cable between the PC host and the MCU-Link USB port on the board
2. Open a serial terminal on PC for the serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running
   the demo.

Running the demo
================
The following lines are printed to the serial terminal when the demo program is executed.
The three values printed each time may be different.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
START DEMO SYSPM
eventcounter = 94
eventcounter = 432342
eventcounter = 953320
END DEMO SYSPM
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~