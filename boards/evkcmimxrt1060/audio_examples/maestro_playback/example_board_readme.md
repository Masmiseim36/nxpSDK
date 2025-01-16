Hardware requirements
=====================
- Micro USB cable
- JTAG/SWD debugger
- MIMXRT1060-EVKC board
- Personal Computer
- Headphones with 3.5 mm stereo jack

Board settings
==============
For Audio board:
1. Insert AUDIO expansion board into J19 to be able to use the CS42448 codec for multichannel
   output.
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

- This development board also supports multi-channel example. The example demonstrates playback
  of raw PCM files from an SD-card with up to 8 channels, 96kHz sample rate and 32 bit width.
    - To enable multi-channel example:
        1. Connect the Audio board to the development board
        2. Define the MULTICHANNEL_EXAMPLE macro in the project settings
        3. Set the DEMO_CODEC_CS42448 macro to 1 in the app_definitions.h file

## Procedure
1. Connect a micro USB cable between the PC host and the debug USB port (J53) on the board
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
4. Insert the headphones into the headphone jack J6 and line in line into J12 on the audio board.
5. Either press the reset button on your board or launch the debugger in your IDE to begin
   running the demo.

**Notes**
- The AAC decoder is only supported in MCUXpresso and ARMGCC.
