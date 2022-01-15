Overview
========
The maestro_demo application demonstrates audio processing on the ARM cortex core
utilizing the Maestro Audio Framework library.

Depending on target platform there are different features of the demo enabled.

    - file decoding and playback
    - EAP effects during file playback

The application is controlled by commands from a shell interface using serial console.

Type "help" to see the command list. Similar description will be displayed on serial console:

    >> help

    "help": List all the registered commands

    "exit": Exit program

    "version": Display component versions

    "file": Perform audio file decode and playback

      USAGE: file [start|stop|pause|update|track|list|info]
        start             Play default (first found) or specified audio track file.
        stop              Stops actual playback.
        pause             Pause actual track or resume if already paused.
        update=<preset>   Apply current EAP parameters without attribute value
                          or switch to preset 1-10
        set=<preset>      Apply current EAP parameters without attribute value
                          or switch to preset 1-10
        list              List audio files on SD card available for playback
        get               Sync actual EAP parameters from library to ATT config structures.
        track=<filename>  Select audio track to play.
        list              List audio files available on mounted SD card.
        info              Prints playback info.
      EXAMPLE: Playback with eap effect: file track <audio_file> eap 3


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
