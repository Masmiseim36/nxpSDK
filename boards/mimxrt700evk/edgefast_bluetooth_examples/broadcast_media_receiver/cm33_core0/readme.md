# broadcast_media_receiver

## Overview
Application demonstrating how to use the broadcast media receiver feature.

There should be three boards: 1 BMS + 1 BMR(left) + 1 BMR(right).
BMS: broadcast stereo audio stream, left channel on first BIS and right channel on another BIS.
BMR: lisen one of BIS channel and render it.

## Prepare the Demo

1.  Open example's project and build it.

2.  Connect a USB cable between the PC host and the OpenSDA USB port on the target board.

3.  Provide 5V voltage for the target board.

4.  Connect speaker to Audio Jack.

5.  Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control

6.  Download the program to the target board.

7.  Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

## Running the demo
The log below shows the output of the example in the terminal window.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Copyright  2022  NXP

BMR>> 
Broadcast Media Receiver.

Please select sink role "left"|"right" use "init" command.

BMR>> init left

BMR@left>> BMR@left>> Bluetooth initialized
Scanning for broadcast sources

[device name]:broadcast_media_sender
connect...
Broadcast source found, waiting for PA sync
PA synced for broadcast sink 202DBE48 with broadcast ID 0xD789F0
Received BASE with 1 subgroups from broadcast sink 202DBE48
Broadcast source PA synced, waiting for BASE
BASE received, waiting for syncable
	Codec: freq 16000, channel count 1, duration 10000, channel alloc 0x00000001, frame len 40, frame blocks per sdu 1
Audio codec configed, waiting for syncable
Syncing to broadcast
Stream 202E3164 started
BMR@left>> 
BMR@left>> 
BMR@left>> 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Procedures to run
1 input "init left" or "init right" to start sink role and start to scan broadcast media sender, audio will start playing after sync to source.
2 input "pause" to stop playing.
3 input "play" to start playing.
4 input "vol_up", "vol_down", "vol_set" to set volume.
5 input "vol_mute", "vol_unmute" to set mute.

Other cmds could be used:
1 "sync_info" used to get iso_interval/sync_delay/pd/ts, and this cmd should be used after the audio start playing.
2 "sync_test_mode" used to set the test mode, and this cmd should be used before init.
3 "set_broadcast_code" used to set broadcast code, and this cmd could be used before init or later.

Note:
1 "exit" command is a shell internal command, only used to exit shell module and could not used to exit demo.

## Supported Boards
- [MIMXRT1170-EVKB](../../_boards/evkbmimxrt1170/edgefast_bluetooth_examples/broadcast_media_receiver/example_board_readme.md)
- [MIMXRT1060-EVKC](../../_boards/evkcmimxrt1060/edgefast_bluetooth_examples/broadcast_media_receiver/example_board_readme.md)
- [MIMXRT700-EVK](../../_boards/mimxrt700evk/edgefast_bluetooth_examples/broadcast_media_receiver/example_board_readme.md)
