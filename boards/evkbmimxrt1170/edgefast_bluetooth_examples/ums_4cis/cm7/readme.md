# ums_4cis

## Overview
Application demonstrating how to use the unicast media sender 4 CIS feature.

There should be three boards: 1 UMS + 1 UMR(front) + 1 UMR(back).
- UMS: unicast stereo audio stream, front_left/right channel on first two CIS and back_left/right on last two CIS.
- UMR: receive 2 of CIS channels and render them.

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

UMS>> 
Unicast Media Sender 4CIS.
Initializing
Initialized

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
UMS>> 	sample_rate: 16000
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
UMS>> 	codec_cfg - sample_rate: 16000, duration: 10000, len: 40
	qos - interval: 10000, framing: 0, phy: 2, sdu: 40, rtn: 2, pd: 40000
LC3 encoder setup done!
new_preset:
	codec_cfg - sample_rate: 16000, duration: 10000, len: 40
	qos - interval: 10000, framing: 0, phy: 2, sdu: 40, rtn: 1, pd: 40000
Creating unicast group
Unicast group created
Please scan and connect the devices you want!
scan
UMS>> Scanning successfully started
[0]: A0:CD:F3:77:E4:11 (public), rssi -38, umr_4cis
[1]: A0:CD:F3:77:E6:D7 (public), rssi -55, umr_4cis
connect 0
UMS>> device selected!
Connecting
Connect first device
MTU exchanged: 23/23
LE Connected: A0:CD:F3:77:E4:11 (public)
MTU exchanged: 65/65
Connected
CSIP discover
CSIP conn 202DB824 discovered set count 1
set 1/1 info:
	sirk: 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 
	set_size: 2
	rank: 1
	lockable: 1
CSIP discovered
Scan another member
member: A0:CD:F3:77:E6:D7 (public), rssi -51, umr_4cis
Member discovered
Connecting
Connect second device
MTU exchanged: 23/23
LE Connected: A0:CD:F3:77:E6:D7 (public)
MTU exchanged: 65/65
Connected
CSIP discover
CSIP conn 202DB9B4 discovered set count 1
set 1/1 info:
	sirk: 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 
	set_size: 2
	rank: 2
	lockable: 1
CSIP discovered
Discover VCS

VCS discover finished
Discover VCS complete.
Discovering sinks

VCS inst 0, volume 229, mute 0
codec_cap 202E6C7C dir 0x01
codec id 0x06 cid 0x0000 vid 0x0000 count 19
data: type 0x01 value_len 2
ff1f
data: type 0x02 value_len 1
02
data: type 0x03 value_len 1
02
data: type 0x04 value_len 4
28007800
data: type 0x05 value_len 1
01
meta: type 0x01 value_len 2
0600
dir 1 loc 3
snk ctx 31 src ctx 0
Sink #0: ep 202E40B0
Sink #0: ep 202E4178
Discover sinks complete: err 0
Sinks discovered
Configuring streams
Audio Stream 20304AE0 configured
Configured sink stream[0]
Audio Stream 20304B04 configured
Configured sink stream[1]
Stream configured
Setting stream QoS
QoS: waiting for 0 streams
Audio Stream 20304AE0 QoS set
Audio Stream 20304B04 QoS set
Stream QoS Set
Enabling streams
Audio Stream 20304AE0 enabled
Audio Stream 20304B04 enabled
Streams enabled
Connecting streams
Audio Stream 20304AE0 connected
Audio Stream 20304AE0 started
Audio Stream 20304B04 connected
Streams connected
Starting streams
Audio Stream 20304B04 started
Streams started
Discover VCS

VCS discover finished
Discover VCS complete.
Discovering sinks

VCS inst 1, volume 229, mute 0
codec_cap 202E6C7C dir 0x01
codec id 0x06 cid 0x0000 vid 0x0000 count 19
data: type 0x01 value_len 2
ff1f
data: type 0x02 value_len 1
02
data: type 0x03 value_len 1
02
data: type 0x04 value_len 4
28007800
data: type 0x05 value_len 1
01
meta: type 0x01 value_len 2
0600
dir 1 loc 30
snk ctx 31 src ctx 0
Sink #1: ep 202E46A8
Sink #1: ep 202E4770
Discover sinks complete: err 0
Sinks discovered
Configuring streams
Audio Stream 20304B28 configured
Configured sink stream[2]
Audio Stream 20304B4C configured
Configured sink stream[3]
Stream configured
Setting stream QoS
QoS: waiting for 1 streams
Audio Stream 20304B28 QoS set
Audio Stream 20304B4C QoS set
Stream QoS Set
Enabling streams
Audio Stream 20304B28 enabled
Audio Stream 20304B4C enabled
Streams enabled
Connecting streams
Audio Stream 20304B28 connected
Audio Stream 20304B28 started
Audio Stream 20304B4C connected
Streams connected
Starting streams
Audio Stream 20304B4C started
Streams started
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Procedures to run
1. input "help" to show command list.
2. make sure UMR left and right are initialized and start advertising.
3. input "wav_open <path>" to open one wav file.
4. input "lc3_preset <name>" to load one lc3 preset.
5. input "scan" to start scan all sink devices, then use "connect" to connect one of the set member.
Another set member will be connected automatically.
6. the audio will start playing after all config done.
7. input "pause" to stop playing.
8. input "play" to start playing.
9. input "vol_up", "vol_down", "vol_set" to set volume of all sinks.
10. input "vol_mute", "vol_unmute" to set mute of all sinks.

Other cmds could be used:
1. "lc3_preset_list" used to list all the lc3 preset this demo support.
2. "sync_info" used to get iso_interval/sync_delay, and this cmd should be used after the audio stream start.
3. "config_pd" used to config the pd, and this cmd should be used before "lc3_preset".
4. "config_packing" used to config iso packing mode, and this cmd should be used before "lc3_preset".
5. "config_conn_param" used to config connection interval, and this cmd should be used before "connect". 
The connection interval should >= N * iso_interval (N equal to iso channel number).

Note:
1. "exit" command is a shell internal command, only used to exit shell module and could not used to exit demo.
2. the "music_16_2.wav" should be 16/24/32bits 2 channel with sample rate 8000/16000/24000/32000/48000.

## Supported Boards
- [MIMXRT1170-EVKB](../../_boards/evkbmimxrt1170/edgefast_bluetooth_examples/ums_4cis/example_board_readme.md)