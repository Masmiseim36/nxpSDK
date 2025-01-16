Hardware requirements
=====================
- Micro USB cable
- JTAG/SWD debugger
- EVK-MIMXRT595 board
- Personal Computer
- Headphones with 3.5 mm stereo jack

Board settings
==============
Note: The I3C Pin configuration in pin_mux.c is verified for default 1.8V, for 3.3V,
need to manually configure slew rate to slow mode for I3C-SCL/SDA.

To enable the example audio using WM8904 codec, connect pins as follows:
- JP7-1        <-->        JP8-2

Prepare the Demo
================
1. Connect headphones to Audio HP / Line-Out connector (J4).
2. Connect a micro USB cable between the PC host and the debug USB port (J40) on the board
3. Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4. Download the program for CM33 core to the target board.
5. Launch the debugger in your IDE to begin running the demo.
6. If building debug configuration, start the xt-ocd daemon and download the program for
   DSP core to the target board.
7. If building debug configuration, launch the Xtensa IDE or xt-gdb debugger to
begin running the demo.

**Notes**
- To be able to build the DSP project, please see the document
'Getting Started with Xplorer for EVK-MIMXRT595.pdf'.
- DSP image can only be debugged using J-Link debugger. See again
'Getting Started with Xplorer for EVK-MIMXRT595.pdf' for more information.

Running the demo CM33
=====================
When the demo runs successfully, the terminal will display the following:
```
    ******************************
    DSP audio framework demo start
    ******************************

    [CM33 Main] Configure WM8904 codec
    [CM33_Main] DSP image copied to DSP TCM
    [CM33_Main][APP_SDCARD_Task] start
    [CM33_Main][APP_DSP_IPC_Task] start
    [CM33_Main][APP_Shell_Task] start

    Copyright  2022  NXP
    >>
```

Demo commands:
```
"help": List all the registered commands

"file": Perform audio file decode and playback on DSP
  USAGE: file [list|stop|<audio_file>]
    list          List audio files on SD card available for playback
    <audio_file>  Select file from SD card and start playback
```

When file command starts playback successfully, the terminal will display following output:
```
    [APP_DSP_IPC_Task] response from DSP, cmd: 12, error: 0
    DSP file playback start
    >>
```

Xtensa IDE log when command is playing a file (mp3/aac/vorbis):
```
    [DSP_Main] File playback start, initial buffer size: 16384

    [DSP Codec] Audio Device Ready

    [DSP Codec] Decoder component started

    [DSP Codec] Setting decode playback format:

    [DSP Codec] Decoder    : mp3_dec

    [DSP Codec] Sample rate: 48000

    [DSP Codec] Bit Width  : 16

    [DSP Codec] Channels   : 2

    [DSP Codec] Renderer component started

    [DSP Codec] Connected XA_DECODER -> XA_RENDERER

    [DSP_ProcessThread] start

    [DSP_BufferThread] start


    [DSP_Main] File playback stop

    [DSP_ProcessThread] exiting

    [DSP_BufferThread] exiting

    [DSP Codec] Audio device closed
```
Xtensa IDE will not show any additional log entry.

Running the demo DSP
===================
Debug configuration:
When the demo runs successfully, the terminal will display the following:
```
    [DSP_Main] Cadence Xtensa Audio Framework

    [DSP_Main] Library Name    : Audio Framework (Hostless)

    [DSP_Main] Library Version : 3.2

    [DSP_Main] API Version     : 3.0



    [DSP_Main] start

    [DSP_Main] established RPMsg link
```

