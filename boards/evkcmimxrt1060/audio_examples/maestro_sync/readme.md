# maestro_sync

## Overview
The maestro_sync application demonstrates the use of synchronous pipelines (Tx and Rx in this
case) processing on the ARM cortex core utilizing the Maestro Audio Framework library.
This feature is useful for testing the latency of the pipeline or implementing
algorithms requiring reference signals (like echo cancellation). The libraries
available in this example (VoiceSeeker) are not featuring AEC (acoustic echo cancellation),
but NXP is offering it in the premium version of the libraries. You can
visit www.nxp.com/voiceseeker for more information.

The demo uses two pipelines running synchronously in a single streamer task:
1. Playback pipeline:
    - Playback of audio data in PCM format stored in flash memory to the speaker.
2. Recording pipline:
    - Record audio data using a microphone.
    - VoiceSeeker processing.
    - Wake words + voice commands recognition.
    - Save VoiceSeeker output to SD card.

The application is controlled by commands from a shell interface using serial console.

Type "help" to see the command list. Similar description will be displayed on serial console:
```
    >> help

    "help": List all the registered commands

    "exit": Exit program

    "version": Display component versions

    "start [nosdcard]": Starts a streamer task.
                  - Initializes the streamer with the Memory->Speaker pipeline and with
                    the Microphone->VoiceSeeker->VIT->SDcard pipeline.
                  - Runs repeatedly until stop command.
         nosdcard - Doesn't use SD card to store data.

    "stop": Stops a running streamer:

    "debug [on|off]": Starts / stops debugging.
                    - Starts / stops saving VoiceSeeker input data (reference and microphone data)
                      to SDRAM.
                    - After the stop command, this data is transferred to the SD card.
```

For custom VIT model generation (defining own wake words and voice commands)
please use https://vit.nxp.com/


## Running the demo
When the example runs successfully, you should see similar output on the serial terminal as below:
```
    *****************************
    Maestro audio sync demo start
    *****************************

    Copyright  2022  NXP
    [APP_SDCARD_Task] start
    [APP_Shell_Task] start

    >> [APP_SDCARD_Task] SD card drive mounted
```

## Supported Boards
- [MIMXRT1170-EVKB](../../_boards/evkbmimxrt1170/audio_examples/maestro_sync/example_board_readme.md)
- [MIMXRT1060-EVKC](../../_boards/evkcmimxrt1060/audio_examples/maestro_sync/example_board_readme.md)
