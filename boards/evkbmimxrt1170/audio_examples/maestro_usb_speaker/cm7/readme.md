# maestro_usb_speaker

## Overview
The maestro_usb_speaker application demonstrates audio processing on the ARM cortex core
utilizing the Maestro Audio Solutions library.

The application is controlled by commands from a shell interface using serial console.

Type "help" to see the command list. Similar description will be displayed on serial console:
```
    >> help

    "help": List all the registered commands

    "exit": Exit program

    "version": Display component versions

    "usb_speaker": Play data from the USB port as an audio 2.0
                   speaker device.
       USAGE:     usb_speaker <seconds>
       <seconds>  Time in seconds how long the application should run.
                   When you enter a negative number the application will
                   run until the board restarts.
       EXAMPLE:   The application will run for 20 seconds: usb_speaker 20
```

After running the "usb_speaker" command, the USB device will be enumerated on your host.
User will see the volume levels obtained from the USB host as in the example below.
This is just an example application. To leverage the values, the demo has to be modified.

**Notes**
1. If the USB device audio speaker example uses an ISO IN feedback endpoint, please attach
   the device to a host like PC which supports feedback function. Otherwise, there might be
   attachment issue or other problems.
2. This example supports UAC 5.1 and UAC 5.1 is disabled by default, this feature can be
   enabled by set macro USB_AUDIO_CHANNEL5_1 as 1U.
    - When playing an 5.1 audio file, the example sends only the front-left and front-right
    channels to the Audio Line Out port (the other channels are ignored), since this example
    only supports on-board codecs with stereo audio output.
3. When device functionality is changed, such as UAC 5.1, please uninstall the previous PC
   driver to make sure the device with changed functionality can run normally.

## Running the demo
When the example runs successfully, you should see similar output on the serial terminal as below:

```
    **********************************************
    Maestro audio USB speaker solutions demo start
    **********************************************

    Copyright  2022  NXP
    [APP_Shell_Task] start

    >> usb_speaker -1

    Starting maestro usb speaker application
    The application will run until the board restarts
    [STREAMER] Message Task started
    Starting playing
    [STREAMER] start usb speaker
    Set Cur Volume : fbd5
```

## Supported Boards
- [MIMXRT1170-EVKB](../../_boards/evkbmimxrt1170/audio_examples/maestro_usb_speaker/example_board_readme.md)
- [MIMXRT1060-EVKC](../../_boards/evkcmimxrt1060/audio_examples/maestro_usb_speaker/example_board_readme.md)
- [LPCXpresso55S69](../../_boards/lpcxpresso55s69/audio_examples/maestro_usb_speaker/example_board_readme.md)
- [MCX-N5XX-EVK](../../_boards/mcxn5xxevk/audio_examples/maestro_usb_speaker/example_board_readme.md)
