Overview
========
The voice_spot_demo application demonstrates Voice Spot initialization and processing.
It gets the audio data from the on-board microphone, looks for a wake word (Hey NXP) and outputs
the audio data to the speaker.

Toolchain supported
===================
- IAR embedded Workbench  9.30.1
- Keil MDK  5.37
- GCC ARM Embedded  10.3.1
- MCUXpresso  11.6.0

Hardware requirements
=====================
- Micro USB cable
- JTAG/SWD debugger
- EVK-MIMXRT595 board sch rev C and higher
- Personal Computer
- Headphones with 3.5 mm stereo jack

Board settings
==============
Note: The I3C Pin configuration in pin_mux.c is verified for default 1.8V, for 3.3V,
need to manually configure slew rate to slow mode for I3C-SCL/SDA.

To enable the example audio using WM8904 codec, connect jumpers as follows:
The 1-2 connected for JP7, JP8, JP27, JP28, JP29.

Prepare the Demo
================
1.  Connect headphones to Audio HP / Line-Out connector (J4).
2.  Connect a micro USB cable between the PC host and the debug USB port (J40) on the board
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program for CM33 core to the target board.
5.  Launch the debugger in your IDE to begin running the demo.
6.  If building debug configuration, download the program for DSP core to the target board.
7.  If building debug configuration, launch the Xtensa IDE or xt-gdb debugger to
begin running the demo.

NOTE: DSP image can only be debugged using J-Link debugger.  See
'Getting Started with Xplorer for EVK-MIMXRT595.pdf' for more information.

Running the demo
================
The following lines are printed to the serial terminal when the demo program is executed.

    Voice spot demo started. Initialize pins and codec on core 'Cortex-M33'
    Configure WM8904 codec
    Pins and codec initialized.
    VoiceSpot example started on core 'nxp_rt500_RI2021_8_newlib'
    rdspVoiceSpot_CreateControl: voicespot_status = 0
    rdspVoiceSpot_CreateInstance: voicespot_status = 0
    rdspVoiceSpot_OpenInstance: voicespot_status = 0
    rdspVoiceSpot_EnableAdaptiveThreshold: voicespot_status = 0
    VoiceSpot library version: 0.22.2.0
    VoiceSpot model version: 0.13.1
    VoiceSpot model string: HeyNXP_en-US_1
    Voice spot init done.Trigger event found: Event = 0, Frame = 13488, class_string = HeyNXP, Score = 1024, trigger_sample = 2697800, estimation_sample = 2697800, start_offset_samples = 12012, start_sample = 2685788, stop_offset_samples = 3212, stop_sample = 2694588


This example transfers data from DMIC to Codec and waits for the wake word. Connect headphone/earphone on "line-out" of the board.
Speak into the DMIC or play audio near the DMIC (U120), and you will hear sound on the left channel of
headphone/earphone.
