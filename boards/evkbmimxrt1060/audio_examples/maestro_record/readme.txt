Overview
========
The maestro_demo application demonstrates audio processing on the ARM cortex core
utilizing the Maestro Audio Framework library.

Depending on target platform there are different features of the demo enabled.

    - loopback from microphone to speaker
    - recording microphone to a file
    - wake words + voice commands recognition

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

For custom VIT model generation (defining own wake words and voice commands) please use https://vit.nxp.com/


Toolchain supported
===================
- IAR embedded Workbench  9.30.1
- GCC ARM Embedded  10.3.1
- MCUXpresso  11.6.0

Hardware requirements
=====================
- Micro USB cable
- JTAG/SWD debugger
- MIMXRT1060-EVKB board
- Personal Computer
- Headphones with 3.5 mm stereo jack
- Audio expansion board AUD-EXP-42448 (REV B)

Board settings
==============
Make sure resistors R368/R347/R349/R365/R363 are removed to be able to use SD-Card.

For Audio expansion board:
1.Insert AUDIO board into J23 if on board codec is not used
2.Uninstall J41
3.Define DEMO_CODEC_CS42448 1 in app_definitions.h
For on board codec:
1.Make sure J41 is installed
2.Define DEMO_CODEC_WM8960 1 in app_definitions.h

Prepare the Demo
================
Note: As the EVKBMIMXRT1060 supports two codecs, a default on board WM8960 codec and another codec CS42448 on audio board, so to support both of the codecs, the example provides options to switch between the two codecs,
- DEMO_CODEC_WM8960, set to 1 if wm8960 is used
- DEMO_CODEC_CS42448, set to 1 if cs42448 is used
Please do not set above macros to 1 together, as the demo supports one codec only.

1.  Connect a micro USB cable between the PC host and the debug USB port on the board
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
4. For the loopback (record_mic audio) and for the file output (record_mic file) the audio stream is as follows:
    Stereo INPUT 1 (J12) -> LINE 1&2 OUTPUT (J6)
    Stereo INPUT 2 (J15) -> LINE 3&4 OUTPUT (J7)
    MIC1 & MIC2 (P1, P2) -> LINE 5&6 OUTPUT (J8)
    Insert the headphones into the different line outputs to hear the inputs.
    To use the Stereo INPUT 1, 2, connect an audio source LINE IN jack.
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
