Hardware requirements
=====================
- Micro USB cable
- JTAG/SWD debugger
- MIMXRT685-AUD-EVK board
- Personal Computer
- One or more headphones with 3.5 mm stereo jack

Board settings
==============
1. Set the hardware jumpers (Tower system/base module) to default settings.
2. Set hardware jumpers JP2 2-3, JP44 1-2 and JP45 1-2.

Prepare the Demo
================
1. Connect all headphones to Audio HP / Line-Out connectors (J4, J50, J51, J52).
2. Connect a micro USB cable between the PC host and the debug USB port (J5) on the board
3. Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4. Download the program for CM33 core to the target board.
5. Launch the debugger in your IDE to begin running the demo.
6. If building debug configuration, start the xt-ocd daemon and download the program for DSP
   core to the target board.
7. If building debug configuration, launch the Xtensa IDE or xt-gdb debugger to
begin running the demo.

**Notes**
- DSP image can only be debugged using J-Link debugger. See again 'Getting Started
  with Xplorer for MIMXRT685-AUD-EVK.pdf' for more information.
- To be able to build the DSP project, please see the document
  'Getting Started with Xplorer for MIMXRT685-AUD-EVK.pdf'.

Running the demo CM33
=====================
When the demo runs successfully, the terminal will display the following:
```
    ******************************
    DSP audio framework demo start
    ******************************

    Configure CS42448 codec
    [APP_SDCARD_Task] start
    [APP_DSP_IPC_Task] start
    [APP_Shell_Task] start

    SHELL build: Dec 18 2019
 * Copyright 2021 NXP
    >>
```

Demo commands:
```
"help": List all the registered commands

"file": Perform audio file decode and playback on DSP
  USAGE: file [list|stop|<audio_file> [<nchannel>]]
    list          List audio files on SD card available for playback
    <audio_file>  Select file from SD card and start playback
    <nchannel>    Select the number of channels (2 or 8 can be selected).
    NOTE: Selected audio file must meet the following parameters:
                  - Sample rate: 96 kHz
                  - Width:       32 bit
```

When file command starts playback successfully, the terminal will display following output:
```
    [APP_DSP_IPC_Task] response from DSP, cmd: 12, error: 0
    DSP file playback start
    >>
```

Xtensa IDE log when command is playing a file (mp3/aac/vorbis):
```
    File playback start, initial buffer size: 16384
    [DSP Codec] Audio Device Ready
    [DSP Codec] Decoder component started
    [DSP Codec] Setting decode playback format:
      Decoder    : mp3_dec
      Sample rate: 16000
      Bit Width  : 16
      Channels   : 2
    [DSP Codec] Renderer component started
    [DSP Codec] Connected XA_DECODER ->  XA_RENDERER
    [DSP_ProcessThread] start
    [DSP_BufferThread] start
```

Xtensa IDE log when command is playing a PCM file:
```
    [DSP_FILE_REN] Audio Device Ready
    [DSP_FILE_REN] post-proc/pcm_gain component started
    [DSP_FILE_REN] post-proc/client_proxy component started
    [DSP_FILE_REN] Connected post-proc/pcm_gain -> post-proc/client_proxy
    [DSP_FILE_REN] renderer component started
    [DSP_FILE_REN] Connected post-proc/client_proxy -> renderer
    [DSP_BufferThread] start
    [DSP_ProcessThread] start
    [DSP_CleanupThread] start3
```

Xtensa IDE will not show any additional log entry.

Running the demo DSP
====================
Debug configuration:
When the demo runs successfully, the terminal will display the following:
```
    Cadence Xtensa Audio Framework
      Library Name    : Audio Framework (Hostless)
      Library Version : 2.6p1
      API Version     : 2.0

    [DSP_Main] start
    [DSP_Main] established RPMsg link
```

