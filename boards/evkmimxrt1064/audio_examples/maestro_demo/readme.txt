Overview
========
The maestro_demo application demonstrates audio processing on the ARM cortex core
utilizing the Maestro Audio Framework library.

Depending on target platform there are different features of the demo enabled.

    - file decoding and playback
    - EAP effects during file playback
    - loopback from microphone to speaker
    - recording microphone to a file
    - wake word + voice recognition

The application is controlled by commands from a shell interface using serial console.

Type "help" to see the command list. Similar description will be displayed on serial console:

    >> help

    "help": List all the registered commands

    "exit": Exit program

    "version": Display component versions

    "file": Perform audio file decode and playback

      USAGE: file [list|<audio_file>|all]
        list          List audio files on SD card available for playback
        <audio_file>  Select file from SD card and start playback
        all           Play all supported audio files from SD card.
        eap           choose one of the options:
                      1: All effect Off, 2: Voice enhancer, 3: Music enhancer
                      4: Auto volume leveler, 5: Loudness max, 6: 3D Concert snd
                      7: Custom, 8: Tone generator, 9: Crossover two way speakers
                      10: Crossover for subwoofer
      EXAMPLE: Playback with eap effect: file <audio_file> eap 3

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
- MCUXpresso  11.4.0

Hardware requirements
=====================
- Micro USB cable
- JTAG/SWD debugger
- EVK-MIMXRT1060 board
- Personal Computer
- Headphones with 3.5 mm stereo jack

Board settings
==============
Please insert the SDCARD into card slot(J20)

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
