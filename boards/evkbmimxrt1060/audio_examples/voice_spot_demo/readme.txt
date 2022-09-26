Overview
========
The voice_spot_demo application demonstrates Voice Spot initialization and processing.
It gets the audio data from the on-board microphone, looks for a wake word (Hey NXP) and outputs
the audio data to the speaker.


Toolchain supported
===================
- MCUXpresso  11.6.0

Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT1060-EVKB board
- Personal Computer
- Headphone(OMTP standard)
- CS42448 Audio board(Not necessary if use on board codec)

Board settings
==============
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

1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
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

VoiceSpot example started!
rdspVoiceSpot_CreateControl: voicespot_status = 0
rdspVoiceSpot_CreateInstance: voicespot_status = 0
rdspVoiceSpot_OpenInstance: voicespot_status = 0
rdspVoiceSpot_EnableAdaptiveThreshold: voicespot_status = 0
VoiceSpot library version: 0.22.2.0
VoiceSpot model version: 0.13.1
VoiceSpot model string: HeyNXP_en-US_1
Voice spot init done.
Trigger event found: Event = 0, Frame = 13488, class_string = HeyNXP, Score = 1024, trigger_sample = 2697800, estimation_sample = 2697800, start_offset_samples = 12012, start_sample = 2685788, stop_offset_samples = 3212, stop_sample = 2694588
