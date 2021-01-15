Overview
========
The maestro_demo application demonstrates audio processing on the ARM cortex core
utilizing the Maestro Audio Solutions library.

The application is controlled by commands from a shell interface using serial console.

Type "help" to see the command list.


Toolchain supported
===================
- IAR embedded Workbench  8.50.9
- MCUXpresso  11.3.0

Hardware requirements
=====================
- Micro USB cable
- JTAG/SWD debugger
- EVK-MIMXRT1060 board
- Personal Computer
- Headphones with 3.5 mm stereo jack

Board settings
==============

Prepare the Demo
================
1.  Connect a micro USB cable between the PC host and the debug USB port (J41) on the board
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Connect the headphones into the headphone jack on EVK-MIMXRT1060 board (J12).
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.
Running the demo
================
When the example runs successfully, you should see similar output on the serial terminal as below:

**********************************
Maestro audio solutions demo start
**********************************

[APP_SDCARD_Task] start
[APP_Shell_Task] start

SHELL build: Nov  5 2020
Copyright  2020  NXP

>> [APP_SDCARD_Task] SD card drive mounted
