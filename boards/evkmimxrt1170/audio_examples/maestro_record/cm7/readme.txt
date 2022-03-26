Overview
========
The maestro_demo application demonstrates audio processing on the ARM cortex core
utilizing the Maestro Audio Framework library.

Depending on target platform there are different features of the demo enabled.

    - loopback from microphone to speaker
    - recording microphone to a file
    - wake word + voice recognition

The application is controlled by commands from a shell interface using serial console.

Type "help" to see the command list. Similar description will be displayed on serial console:

    >> help

    "help": List all the registered commands

    "exit": Exit program

    "version": Display component versions

    "record_mic": Record MIC audio and either:
     - perform voice recognition (VIT)
     - playback on WM8904 codec
     - store samples to file.

     USAGE: record_mic [audio|file|vit] 20 [en|cn]
     The number defines length of recording in seconds.
     For voice recognition say supported WakeWord and in 3s frame supported command.
     Please note that this VIT demo is near-field and uses 1 on-board microphone.
     NOTE: this command returns to shell after record finished.


Toolchain supported
===================
- IAR embedded Workbench  9.20.2
- GCC ARM Embedded  10.3.1
- MCUXpresso  11.5.0

Hardware requirements
=====================
- Micro USB cable
- JTAG/SWD debugger
- EVK-MIMXRT1170 board
- Personal Computer
- Headphones with 3.5 mm stereo jack
- SD card

Board settings
==============
To make the examples work,
1.Please remove below resistors if on board wifi chip is not DNP:
R228,R229,R232,R234.
2.Please make sure R136 is weld for GPIO card detect.

Prepare the Demo
================
1.  Connect a micro USB cable between the PC host and the debug USB port (J11) on the board
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Connect the headphones into the headphone jack on EVK-MIMXRT1170 board (J33).
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
