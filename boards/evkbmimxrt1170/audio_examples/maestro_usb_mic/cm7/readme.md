# maestro_usb_mic

## Overview
The maestro_usb_mic application demonstrates audio processing on the ARM cortex core
utilizing the Maestro Audio Solutions library.

The application is controlled by commands from a shell interface using serial console.

Type "help" to see the command list. Similar description will be displayed on serial console:
```
    >> help

    "help": List all the registered commands

    "exit": Exit program

    "version": Display component versions

    "usb_mic": Record MIC audio and playback to the USB port as an audio 2.0
                microphone device.
        USAGE:     usb_mic <seconds>
        <seconds>  Time in seconds how long the application should run.
                When you enter a negative number the application will
                run until the board restarts.
        EXAMPLE:   The application will run for 20 seconds: usb_mic 20
    >>
```

After running the "usb_mic" command, the USB device will be enumerated on your host.
User will see the volume levels obtained from the USB host as in the example below.
This is just an example application. To leverage the values, the demo has to be modified.

**Notes**
1. When device functionality is changed, please uninstall the previous PC driver to make
   sure the device with changed functionality can run normally.
2. If you're having audio problems on Windows 10 for recorder, please disable signal
   enhancement as the following if it is enabled and have a try again.

## Running the demo
When the example runs successfully, you should see similar output on the serial terminal as below:
```
    *************************************************
    Maestro audio USB microphone solutions demo start
    *************************************************

    Copyright  2022  NXP
    [APP_Shell_Task] start

    >> usb_mic -1

    Starting maestro usb microphone application
    The application will run until the board restarts
    [STREAMER] Message Task started
    Starting recording
    [STREAMER] start usb microphone
    Set Cur Volume : 1f00
```

## Supported Boards
- [MIMXRT1170-EVKB](../../_boards/evkbmimxrt1170/audio_examples/maestro_usb_mic/example_board_readme.md)
- [MIMXRT1060-EVKC](../../_boards/evkcmimxrt1060/audio_examples/maestro_usb_mic/example_board_readme.md)
- [LPCXpresso55S69](../../_boards/lpcxpresso55s69/audio_examples/maestro_usb_mic/example_board_readme.md)
- [MCX-N5XX-EVK](../../_boards/mcxn5xxevk/audio_examples/maestro_usb_mic/example_board_readme.md)
