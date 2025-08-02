# bms_4bis

## Overview
Application demonstrating how to use the broadcast media sender 4 BIS feature.

There should be three boards: 1 BMS + 1 BMR(front) + 1 BMR(back).
1. BMS: broadcast stereo audio stream, front_left/right channel on first two BIS and back_left/right on last two BIS.
2. BMR: lisen 2 of BIS channels and render them.

Note:
1. 4BIS requires higher performance, and the debug project cannot meet the needs, so here we only provide the release version of the project for this demonstration.
2. The MCUX project is not available due to performance issues.

## Prepare the Demo

1.  Open example's project and build it.

2.  Connect a USB cable between the PC host and the OpenSDA USB port on the target board.

3.  Provide 5V voltage for the target board.

4.  Save a wav music file to a USB disk's root directory and named as "music_16_2.wav".

5.  Connect the USB disk to USB OTG port.

6.  Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control

7.  Download the program to the target board.

8.  Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

## Running the demo
The log below shows the output of the example in the terminal window.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Copyright  2024  NXP

BMS>> 
Broadcast Media Sender 4BIS.
Bluetooth initialized

wav file list:
1, 1:/trangle_44100_2ch_16bits.wav
2, 1:/chrip_48000_2ch_16bits_0_80.wav
3, 1:/music_16_2.wav
4, 1:/music_8000_2ch_16bits.wav
5, 1:/music_16000_2ch_16bits.wav
6, 1:/music_24000_2ch_16bits.wav
7, 1:/music_32000_2ch_16bits.wav
8, 1:/music_44100_2ch_16bits.wav
9, 1:/music_48000_2ch_16bits.wav
10, 1:/play_1ksin_8k_16b.wav
11, 1:/play_1ksin_32k_16b.wav
12, 1:/play_1ksin_48k_16b.wav
13, 1:/sine_16_2.wav
14, 1:/sine_8000_2ch_16bits.wav
15, 1:/sine_8000_2ch_16bits_0_75.wav
16, 1:/sine_16000_2ch_16bits.wav
17, 1:/sine_16000_2ch_16bits_0_75.wav
18, 1:/sine_16000_2ch_24bits.wav
19, 1:/sine_16000_2ch_32bits.wav
20, 1:/sine_24000_2ch_16bits.wav
21, 1:/sine_32000_2ch_16bits.wav
22, 1:/sine_32000_2ch_16bits_0_75.wav
23, 1:/sine_44100_2ch_16bits.wav
24, 1:/sine_48000_2ch_16bits.wav
25, 1:/sine_48000_2ch_16bits_0_75.wav
26, 1:/sine_48000_2ch_16bits_100ms_sine_900ms_silence.wav
wav file list complete!

Please open the wav file you want use "wav_open <path>" command.
wav_open 1:/music_16000_2ch_16bits.wav
wav file info:
BMS>> 	sample_rate: 16000
	channels: 2
	bits: 16
	size: 1163600
	samples: 290900

lc3 preset list:
16_2_1:
	codec_cfg - sample_rate: 16000, duration: 10000, len: 40
	qos - interval: 10000, framing: 0, phy: 2, sdu: 40, rtn: 2, pd: 40000

Please select lc3 preset use "lc3_preset <name>" command.
lc3_preset 16_2_1
16_2_1:
BMS>> 	codec_cfg - sample_rate: 16000, duration: 10000, len: 40
	qos - interval: 10000, framing: 0, phy: 2, sdu: 40, rtn: 2, pd: 40000
new_preset:
	codec_cfg - sample_rate: 16000, duration: 10000, len: 40
	qos - interval: 10000, framing: 0, phy: 2, sdu: 40, rtn: 1, pd: 40000
LC3 encoder setup done!
Creating broadcast source
Creating broadcast source with 1 subgroups with 4 streams
Starting broadcast source
Broadcast source started
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Procedures to run
1. input "help" to show command list
2. input "wav_open <path>" open the wav file.
3. input "lc3_preset <name>" load lc3 preset, then the broadcast will start.
4. input "pause" to stop the broadcast.
5. input "play" to start broadcast.

Other cmds could be used:
1. "lc3_preset_list" used to list all the lc3 preset this demo support.
2. "sync_info" used to get iso_interval/sync_delay, and this cmd should be used after the audio stream start.
3. "config_pd" used to config the pd, and this cmd should be used before "lc3_preset".
4. "config_packing" used to config iso packing mode, and this cmd should be used before "lc3_preset".
5. "set_broadcast_code" used to set broadcast code, and this cmd should be used before "lc3_preset".

Note:
1. "exit" command is a shell internal command, only used to exit shell module and could not used to exit demo.
2. the "music_16_2.wav" should be 16/24/32bits 2 channel with sample rate 8000/16000/24000/32000/48000.

## Supported Boards
- [MIMXRT1170-EVKB](../../_boards/evkbmimxrt1170/edgefast_bluetooth_examples/bms_4bis/example_board_readme.md)
