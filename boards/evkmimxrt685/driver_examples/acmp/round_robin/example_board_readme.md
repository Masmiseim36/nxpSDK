Hardware requirements
=====================
- Micro USB cable
- EVK-MIMXRT685 board
- Personal Computer

Board settings
==============
This example project uses ACMP instance 0 to compare the voltage signal input from channel A(J1-9)
with the voltage signal(half of VDDA) output by ACMP's internal DAC. In order to observe the low
power wakeup phenomenon, channel A(J1-9) should be connected to GND signal(J25-1) on the board before
the example running. Example will exit deep sleep mode when channel A(J1-9) is disconnected from GND signal
and the disconnected time is last for about 1s.

Prepare the Demo
================
1.  Connect a micro USB cable between the PC host and the CMSIS DAP USB port (J5) on the board
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Launch the debugger in your IDE to begin running the demo.

Running the demo
================
${ANCHOR}
