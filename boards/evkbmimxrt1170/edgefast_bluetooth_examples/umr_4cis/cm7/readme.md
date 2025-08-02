# umr_4cis

## Overview
Application demonstrating how to use the unicast media receiver 4 CIS feature.

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

UMR>> 
Unicast Media Receiver 4CIS.

Please select sink role "front"|"back" use "init" command.

UMR>> init front

UMR@front>> UMR@front>> Bluetooth initialized
Set info:
	sirk: 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 
	set_size: 2
	rank: 1
	lockable: 1
Location successfully set
Supported contexts successfully set
Available contexts successfully set
Advertising successfully started
Connected: A0:CD:F3:77:E5:8D (public)
Security changed: A0:CD:F3:77:E5:8D (public) level 2 (error 0)
MCS server discover:
MCS server discovered.
ASE Codec Config: conn 202EE80C ep 202F10CC dir 1
codec_cfg 0x06 cid 0x0000 vid 0x0000 count 16
data: type 0x01 value_len 1
03
data: type 0x02 value_len 1
01
data: type 0x03 value_len 4
01000000
data: type 0x04 value_len 2
2800
  Frequency: 16000 Hz
  Frame Duration: 10000 us
  Channel allocation: 0x1
  Octets per frame: 40 (negative means value not pressent)
  Frames per SDU: 1
ASE Codec Config stream 202FD480
ASE Codec Config: conn 202EE80C ep 202F1178 dir 1
codec_cfg 0x06 cid 0x0000 vid 0x0000 count 16
data: type 0x01 value_len 1
03
data: type 0x02 value_len 1
01
data: type 0x03 value_len 4
02000000
data: type 0x04 value_len 2
2800
  Frequency: 16000 Hz
  Frame Duration: 10000 us
  Channel allocation: 0x2
  Octets per frame: 40 (negative means value not pressent)
  Frames per SDU: 1
ASE Codec Config stream 202FD4A0
QoS: stream 202FD480 qos 202F2478
QoS: interval 10000 framing 0x00 phy 0x02 sdu 40 rtn 1 latency 10 pd 40000
QoS: stream 202FD4A0 qos 202F2478
QoS: interval 10000 framing 0x00 phy 0x02 sdu 40 rtn 1 latency 10 pd 40000
Enable: stream 202FD480 meta_len 4
	Codec: freq 16000, channel count 1, duration 10000, channel alloc 0x00000001, frame len 40, frame blocks per sdu 1
Enable: stream 202FD4A0 meta_len 4
	Codec: freq 16000, channel count 1, duration 10000, channel alloc 0x00000002, frame len 40, frame blocks per sdu 1
Unicast stream started
Stream 202FD480 started
Stream 202FD4A0 started
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Procedures to run
1. input "help" to show command list
2. input "init left" or "init right" to start sink role and start to advertise, audio will start playing after connect and configure finished.
3. input "pause" to stop playing.
4. input "play" to start playing.
5. input "vol_up", "vol_down", "vol_set" to set volume.
6. input "vol_mute", "vol_unmute" to set mute.

Other cmds could be used:
1. "sync_info" used to get iso_interval/sync_delay/pd/ts, and this cmd should be used after the audio stream start.
2. "sync_test_mode" used to set the test mode, and this cmd should be used before init.
3. "set_sirk" used to set sirk, and this cmd should be used before init.

Note:
1. "exit" command is a shell internal command, only used to exit shell module and could not used to exit demo.
2. There is a delay in synchronizing volume/mute status between receivers. Setting a smaller connection interval can reduce the delay, but may introduce other timing issues.

## Supported Boards
- [MIMXRT1170-EVKB](../../_boards/evkbmimxrt1170/edgefast_bluetooth_examples/umr_4cis/example_board_readme.md)
