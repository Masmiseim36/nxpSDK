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
- MIMXRT1060-EVKB board
- Personal Computer
- Headphones with 3.5 mm stereo jack

Board settings
==============
Make sure resistors R368/R347/R349/R365/R363 are removed to be able to use SD-Card.

For Audio board:
1.Insert AUDIO board into J23 if on board codec is not used
2.Uninstall J41
For on board codec:
2.Make sure J41 is installed

Prepare the Demo
================
Note: As the EVKBMIMXRT1060 support two codecs, a default on board WM8960 codec and another codec CS42448 on audio board, so to support both of the codecs, the example provide options to switch between the two codecs,
- DEMO_CODEC_WM8960, set to 1 if wm8960 used
- DEMO_CODEC_CS42448, set to 1 if cs42448 used
Please do not set above macros to 1 together, as the demo support one codec only.

1.  Connect a micro USB cable between the PC host and the debug USB port (J41) on the board
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
Steps for WM8960:
4. Insert the headphones into the headphone jack on MIMXRT1060-EVKB board (J34).
Steps for CS42448:
4. Insert the headphones into the headphone jack J6 and line in line into J12 on the audio board.
5. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.
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
