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
- IAR embedded Workbench  9.10.2
- GCC ARM Embedded  10.2.1
- MCUXpresso  11.5.0

Hardware requirements
=====================
- Micro USB cable
- JTAG/SWD debugger
- MIMXRT1160-EVK board
- Personal Computer
- Headphones (OMTP standard)
- SD card

Board settings
==============
Please make sure R136 is weld for GPIO card detect.

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.	Insert SD card to card slot
4.  Download the program to the target board.
5.  Insert the headphones into the headphone jack on MIMXRT1160-EVK board (J33).
6.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.
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
