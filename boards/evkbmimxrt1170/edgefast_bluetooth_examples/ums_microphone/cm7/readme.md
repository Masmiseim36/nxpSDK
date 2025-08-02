# ums_microphone

## Overview
Application demonstrating how to use the unicast media sender feature.

The stream source of UMS is came from microphone.

There should be four boards: 1 ums_microphone + 1 umr2bms bridge + 1 BMR(left) + 1 BMR(right).
- ums_microphone: unicast stereo audio stream, left channel on first CIS and right channel on another CIS.
- umr2bms: combine UMR and BMS features. For UMR, it provides two ASEs, and receives two CIS channel streams from ums_microphone.
    - For BMS, it provides two ASEs. It relays the streams received from UMR and sends them to BIS channels.
BMR: It receives stream from one of BIS channel and render it.

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
Copyright  2024  NXP

UMS>>
Unicast Media Sender.
Initializing
Initialized

Please select lc3 preset use "lc3_preset <name>" command.
lc3_
lc3_preset_list    lc3_preset
UMS>> lc3_preset 48_2_2
48_2_2:
UMS>>   codec_cfg - sample_rate: 48000, duration: 10000, len: 100
        qos - interval: 10000, framing: 0, phy: 2, sdu: 100, rtn: 13, pd: 40000
LC3 encoder setup done!
Creating unicast group
Unicast group created
Please scan and connect the devices you want!
scan
UMS>> Scanning successfully started
[0]: 68:AB:BC:8E:99:FD (public), rssi -66,
[1]: A0:CD:F3:77:E6:15 (public), rssi -33, umr2bms
[2]: 43:72:65:69:B4:0A (random), rssi -44,
c[3]: 7A:6B:86:3A:6F:AC (random), rssi -55,
onnect 1
UMS>> device selected!
Connecting
Connect first device
MTU exchanged: 23/23
LE Connected: A0:CD:F3:77:E6:15 (public)
MTU exchanged: 65/196
Connected
Discover VCS

VCS discover finished
Discover VCS complete.
Discovering sinks

VCS inst 0, volume 229, mute 0
codec_cap 202F624C dir 0x01
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
Sink: ep 202D76F4
Sink: ep 202D77BC
Discover sinks complete: err 0
Sinks discovered
Configuring streams
Audio Stream 202ECE54 configured
Configured sink sinks[0]
Audio Stream 202ECEE4 configured
Configured sink sinks[1]
Stream configured
Setting stream QoS
QoS: waiting for 0 sink
Audio Stream 202ECE54 QoS set
Audio Stream 202ECEE4 QoS set
QOS Set sink sinks[0]
QoS: waiting for 1 sink
QOS Set sink sinks[1]
Stream QoS Set
Enabling streams
Audio Stream 202ECE54 enabled
Init Audio SAI and CODEC, samplingRate :48000  bitWidth:16
Set default headphone volume 70
Enabled sink sinks[0]
Audio Stream 202ECEE4 enabled
Enabled sink sinks[1]
Streams enabled
Connecting streams
Audio Stream 202ECE54 connected
Connect sink sinks[0]
Audio Stream 202ECE54 started
Audio Stream 202ECEE4 connected
Connect sink sinks[1]
Streams connected
Starting streams
Audio Stream 202ECEE4 started
Streams started
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Procedures to run
1. input "help" to show command list
2. input "lc3_preset <name>" to load one lc3 preset.
3. input "scan" to start scan all sink devices, then use "connect" to connect the device.
4. the audio will start playing after all config done.

## Supported Boards
- [MIMXRT1170-EVKB](../../_boards/evkbmimxrt1170/edgefast_bluetooth_examples/unicast_media_sender/example_board_readme.md)
- [MIMXRT1060-EVKC](../../_boards/evkcmimxrt1060/edgefast_bluetooth_examples/unicast_media_sender/example_board_readme.md)
- [MIMXRT700-EVK](../../_boards/mimxrt700evk/edgefast_bluetooth_examples/unicast_media_sender/example_board_readme.md)
