# bmr_4bis

## Overview
Application demonstrating how to use the broadcast media receiver 4BIS feature.

There should be three boards: 1 BMS + 1 BMR(front) + 1 BMR(back).
BMS: broadcast stereo audio stream, front_left/right channel on first two 

BIS and back_left/right on last two BIS.

BMR: lisen 2 of BIS channels and render them.

Note:
1. 4BIS requires higher performance, and the debug project cannot meet the needs, so here we only provide the release version of the project for this demonstration.
2. The MCUX project is not available due to performance issues.

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

Copyright  2024  NXP

BMR>> 
Broadcast Media Receiver 4BIS.

Please select sink role "front"|"back" use "init" command.

BMR>> init front

BMR@front>> BMR@front>> Bluetooth initialized
Scanning for broadcast sources

[device name]:bms_4bis
connect...
Broadcast source found, waiting for PA sync
Attempting to PA sync to the broadcaster with id 0x8D1379
Waiting for PA synced

[device name]:bms_4bis
connect...

[device name]:bms_4bis
connect...
PA synced for sync 202F2AB0 with sid 0x00
Broadcast source PA synced, creating Broadcast Sink
Broadcast Sink created, waiting for BASE
Received BASE with 1 subgroups from broadcast sink 202F45C0
codec_qos - interval: 10000, framing: 0, phy: 2, sdu: 40, rtn: 0, pd: 40000
BASE received, waiting for syncable
	Codec: freq 16000, channel count 2, duration 10000, channel alloc 0x00000003, frame len 40, frame blocks per sdu 1
Audio codec configed, waiting for syncable
Syncing to broadcast
Stream 20304788 started
Stream 203047A8 started
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Procedures to run
1. input "init left" or "init right" to start sink role and start to scan broadcast media sender, audio will start playing after sync to source.
2. input "pause" to stop playing.
3. input "play" to start playing.
4. input "vol_up", "vol_down", "vol_set" to set volume.
5. input "vol_mute", "vol_unmute" to set mute.

Other cmds could be used:
1. "sync_info" used to get iso_interval/sync_delay/pd/ts, and this cmd should be used after the audio start playing.
2. "sync_test_mode" used to set the test mode, and this cmd should be used before init.
3. "set_broadcast_code" used to set broadcast code, and this cmd could be used before init or later.

Note:
1. "exit" command is a shell internal command, only used to exit shell module and could not used to exit demo.

## Supported Boards
- [MIMXRT1170-EVKB](../../_boards/evkbmimxrt1170/edgefast_bluetooth_examples/bmr_4bis/example_board_readme.md)