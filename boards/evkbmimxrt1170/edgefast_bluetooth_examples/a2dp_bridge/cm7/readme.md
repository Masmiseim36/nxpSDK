# unicast_media_sender

## Overview
Application demonstrating how to use the a2dp sink bridge feature.

There should be four boards for a2dp_bridge: 1 a2dp_source + 1 a2dp_bridge(UMS) + 1 UMR(left) + 1 UMR(right).
- a2dp_source: Sends stereo audio stream to a2dp sink(bridge).
- a2dp_bridge: Recive stereo audio stream from a2dp soruce, then send stereo audio stream to unicast media source, left channel on first CIS and right channel on another CIS.
- UMR: receive one of CIS channel and render it.

## Prepare the Demo

1.  Open example's project and build it.

2.  Connect a USB cable between the PC host and the OpenSDA USB port on the target board.

3.  Provide 5V voltage for the target board.

4.  Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control

5.  Download the program to the target board.

6.  Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

## Running the demo
The log below shows the output of the example in the terminal window.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Bluetooth A2dp Bridge demo start...
Bluetooth initialized
BR/EDR set connectable and discoverable done
BR Connected
BR Security changed: D0:17:69:EE:69:71 level 2
a2dp connected success
a2dp configure sample rate 48000Hz, ch 2
a2dp start playing
Switch role for D0:17:69:EE:69:71, err 0

 * Copyright 2025 NXP

UMS>>
Unicast Media Sender.
Initializing
Initialized

Please select lc3 preset use "lc3_preset <name>" command.
BR Security changed: D0:17:69:EE:69:71 level 2

UMS>>
UMS>>
UMS>> lc3_preset 48_2_1
48_2_1:
UMS>>   codec_cfg - sample_rate: 48000, duration: 10000, len: 100
        qos - interval: 10000, framing: 0, phy: 2, sdu: 100, rtn: 5, pd: 40000
LC3 encoder setup done!
Creating unicast group
Unicast group created
Please scan and connect the devices you want!

UMS>>
UMS>> scan
Scanning successfully started
UMS>> [0]: CC:F8:26:F6:87:8C (public), rssi -61, Galaxy Buds2 Pro

UMS>>
UMS>>
UMS>> connect 0[1]: CC:F8:26:ED:E7:82 (public), rssi -70, Galaxy Buds2 Pro

UMS>> device selected!
Connecting
Connect first device
MTU exchanged: 23/23
LE Connected: CC:F8:26:F6:87:8C (public)
MTU exchanged: 196/196
Connected
CSIP discover
CSIP conn 202DD7B8 discovered set count 1
set 1/1 info:
        sirk: cc f8 26 f6 87 8c cc f8 26 ed e7 82 cc f8 26 f6
        set_size: 2
        rank: 1
        lockable: 1
CSIP discovered
Scan another member
member: CC:F8:26:ED:E7:82 (public), rssi -50, Galaxy Buds2 Pro
Member discovered
Connecting
Connect second device
MTU exchanged: 23/23
LE Connected: CC:F8:26:ED:E7:82 (public)
MTU exchanged: 196/196
Connected
CSIP discover
CSIP conn 202DD8B4 discovered set count 1
set 1/1 info:
        sirk: cc f8 26 f6 87 8c cc f8 26 ed e7 82 cc f8 26 f6
        set_size: 2
        rank: 2
        lockable: 1
CSIP discovered
Discover VCS

VCS discover finished
Discover VCS complete.
Discovering sinks
codec_cap 2001A21C dir 0x01
codec id 0x06 cid 0x0000 vid 0x0000 count 19
data: type 0x01 value_len 2
b400
data: type 0x02 value_len 1
03
data: type 0x03 value_len 1
01
data: type 0x04 value_len 4
1a009b00
data: type 0x05 value_len 1
01
meta: type 0x01 value_len 2
0700
dir 1 loc 802
snk ctx 4095 src ctx 623
Sink #0: ep 202D9D10
Sink #0: ep 202D9DD8
Discover sinks complete: err 0
Sinks discovered
Configuring streams
Audio Stream 202F8964 configured
Configured sink stream[0]
Stream configured
Setting stream QoS
QoS: waiting for 0 streams
Audio Stream 202F8964 QoS set
Stream QoS Set
Enabling streams
snk ctx 4091 src ctx 623
Audio Stream 202F8964 enabled
Streams enabled
Starting streams
Audio Stream 202F8964 started
Streams started
Discover VCS

VCS discover finished
Discover VCS complete.
Discovering sinks
codec_cap 2001A21C dir 0x01
codec id 0x06 cid 0x0000 vid 0x0000 count 19
data: type 0x01 value_len 2
b400
data: type 0x02 value_len 1
03
data: type 0x03 value_len 1
01
data: type 0x04 value_len 4
1a009b00
data: type 0x05 value_len 1
01
meta: type 0x01 value_len 2
0700
dir 1 loc 401
snk ctx 4095 src ctx 623
Sink #1: ep 202DA188
Sink #1: ep 202DA250
Discover sinks complete: err 0
Sinks discovered
Configuring streams
Audio Stream 202F8988 configured
Configured sink stream[1]
Stream configured
Setting stream QoS
QoS: waiting for 1 streams
Audio Stream 202F8988 QoS set
Stream QoS Set
Enabling streams
snk ctx 4091 src ctx 623
Audio Stream 202F8988 enabled
Streams enabled
Starting streams
Audio Stream 202F8988 started
Streams started
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Procedures to run a2dp_bridge
1. make sure UMR left and right are initialized and start advertising.
2. wait a2dp_source connect and config the a2dp stream.
3. input "lc3_preset <name>" to load one lc3 preset.
4. input "scan" to start scan all sink devices, then use "connect" to connect one of the set member.
Another set member will be connected automatically.
5. the audio will start playing after all config done.
6. input "pause" to stop playing.
7. input "play" to start playing.
8. input "vol_up", "vol_down", "vol_set" to set volume of all sinks.
9. input "vol_mute", "vol_unmute" to set mute of all sinks.

Other cmds could be used:
1. "lc3_preset_list" used to list all the lc3 preset this demo support.
2. "sync_info" used to get iso_interval/sync_delay, and this cmd should be used after the audio stream start.
3. "config_rtn" used to config the rtn, and this cmd should be used before "lc3_preset".
4. "config_pd" used to config the pd, and this cmd should be used before "lc3_preset".
5. "config_phy" used to config the phy, and this cmd should be used before "lc3_preset".
6. "config_packing" used to config iso packing mode, and this cmd should be used before "lc3_preset".
7. "config_conn_param" used to config connection interval, and this cmd should be used before "connect". 
The connection interval should >= N * iso_interval (N equal to iso channel number).

## Supported Boards
- [MIMXRT1170-EVKB](../../_boards/evkbmimxrt1170/edgefast_bluetooth_examples/a2dp_bridge/example_board_readme.md)
