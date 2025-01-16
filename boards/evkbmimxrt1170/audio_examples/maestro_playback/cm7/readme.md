# maestro_playback

## Overview
The maestro_playback application demonstrates audio processing on the ARM cortex core
utilizing the Maestro Audio Framework library.

Depending on target platform there are different features of the demo enabled.

    - File decoding and playback
    - Multi-channel playback

The application is controlled by commands from a shell interface using serial console.

```
Type "help" to see the command list. Similar description will be displayed on serial console:

    >> help

    "help": List all the registered commands

    "exit": Exit program

    "version": Display component versions

    "file": Perform audio file decode and playback

      USAGE: file [stop|pause|volume|seek|play|list|info]
        stop              Stops actual playback.
        pause             Pause actual track or resume if already paused.
        volume=<volume>   Set volume. The volume can be set from 0 to 100.
        seek=<seek_time>  Seek currently paused track. Seek time is absolute time in milliseconds.
        play=<filename>   Select audio track to play.
        list              List audio files available on mounted SD card.
        info              Prints playback info.
```

## Running the demo
When the example runs successfully, you should see similar output on the serial
terminal as below:
```
    *********************************
    Maestro audio playback demo start
    *********************************

    [APP_Main_Task] started

    Copyright  2022  NXP
    [APP_SDCARD_Task] start
    [APP_Shell_Task] start

    >> [APP_SDCARD_Task] SD card drive mounted
```

##  Known issues
1. MP3 decoder has issues with some of the files. One of the channels can be sometimes
   distorted or missing parts of the signal.
2. Opus decoder doesn't support all the combinations of frame sizes and sample rates.
   The application might crash when playing an unspupported file.

## Supported Boards
- [MIMXRT1170-EVKB](../../_boards/evkbmimxrt1170/audio_examples/maestro_playback/example_board_readme.md)
- [MIMXRT1060-EVKC](../../_boards/evkcmimxrt1060/audio_examples/maestro_playback/example_board_readme.md)
- [LPCXpresso55S69](../../_boards/lpcxpresso55s69/audio_examples/maestro_playback/example_board_readme.md)
- [MCX-N5XX-EVK](../../_boards/mcxn5xxevk/audio_examples/maestro_playback/example_board_readme.md)
