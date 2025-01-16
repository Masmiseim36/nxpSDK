Hardware requirements
=====================
- Micro USB cable
- JTAG/SWD debugger
- MIMXRT1060-EVKC board
- Personal Computer
- Headphones with 3.5 mm stereo jack
- Audio expansion board AUD-EXP-42448 (REV B)

Board settings
==============
For Audio expansion board:
1. Insert AUDIO board into J19 if on board codec is not used
2. Uninstall J99
3. Define DEMO_CODEC_CS42448 1 in app_definitions.h

For on board codec:
1. Make sure J99 is installed
2. Define DEMO_CODEC_WM8962 1 in app_definitions.h

Prepare the Demo
================
## Macros settings
- EVKCMIMXRT1060 supports two codecs. Default on board WM8962 codec and additional
codec CS42448 on audio board. To support both of the codecs, the example provides options
to switch between them using macros, located in app_definitions.h:
    - DEMO_CODEC_WM8962, set to 1 if wm8962 is used (on board codec)
    - DEMO_CODEC_CS42448, set to 1 if cs42448 is used (audio board codec)
    Please do not set both macros to 1 together, as the demo supports using one codec at a time.

- In order to enable VoiceSeeker it is necessary to add VOICE_SEEKER_PROC to preprocessor defines
on project level and either connect the AUD-EXP board or provide another multi-channel data to
VoiceSeeker.

## Procedure
1. Connect a micro USB cable between the PC host and the debug USB port on the board
2. Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3. Download the program to the target board.
Steps for WM8962:
4. Insert the headphones into the headphone jack on MIMXRT1060-EVKC board (J101).
Steps for CS42448:
4. For the loopback (record_mic audio) and for the file output (record_mic file) the audio
   stream is as follows:
    - Stereo INPUT 1 (J12) -> LINE 1&2 OUTPUT (J6)
    - Stereo INPUT 2 (J15) -> LINE 3&4 OUTPUT (J7)
    - MIC1 & MIC2 (P1, P2) -> LINE 5&6 OUTPUT (J8)
    - Insert the headphones into the different line outputs to hear the inputs.
    - To use the Stereo INPUT 1, 2, connect an audio source LINE IN jack.
    - Please have in mind that the resulting pcm file in case of the file output has following
      parameters:
    - WM8962 codec:  2 channels, 16kHz, 16bit width
    - CS42448 codec: 8 channels, 16kHz, 32bit width
5. Either press the reset button on your board or launch the debugger in your IDE to begin
   running the demo.

