# tmap_central

## Overview
Application demonstrating how to use the tmap central feature.

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
Bluetooth initialized
Initializing TMAP and setting role
CAP initialized
VCP initialized
MCP initialized
CCP initialized
Scanning successfully started
[DEVICE]: 41:77:CE:FE:99:1B (random), [AD]: 1 data_len 1
[AD]: 255 data_len 22
[AD]: 3 data_len 2
[DEVICE]: 56:CF:9F:4C:98:40 (random), [AD]: 1 data_len 1
[AD]: 25 data_len 2
[AD]: 2 data_len 2
[AD]: 46 data_len 6
[AD]: 22 data_len 4
Found TMAS in peer adv data!
Attempt to connect!
MTU exchanged: 23/23
Connected: 56:CF:9F:4C:98:40 (random)
MTU exchanged: 65/65
Security changed: 0
TMAS discovery done
Error sending mute command!
Found CAS
codec_capabilities 202CE99C dir 0x01
codec 0x06 cid 0x0000 vid 0x0000 count 4
data #0: type 0x01 len 1

data #1: type 0x02 len 1

data #2: type 0x03 len 4
010000
data #3: type 0x04 len 2
1e
meta #0: type 0x02 len 2
07
codec_capabilities 202CE99C dir 0x01
codec 0x06 cid 0x0000 vid 0x0000 count 4
data #0: type 0x01 len 1

data #1: type 0x02 len 1

data #2: type 0x03 len 4
010000
data #3: type 0x04 len 2
3c
meta #0: type 0x02 len 2
07
codec_capabilities 202CE99C dir 0x01
codec 0x06 cid 0x0000 vid 0x0000 count 4
data #0: type 0x01 len 1

data #1: type 0x02 len 1

data #2: type 0x03 len 4
010000
data #3: type 0x04 len 2
50
meta #0: type 0x02 len 2
07
codec_capabilities 202CE99C dir 0x01
codec 0x06 cid 0x0000 vid 0x0000 count 4
data #0: type 0x01 len 1

data #1: type 0x02 len 1

data #2: type 0x03 len 4
010000
data #3: type 0x04 len 2
4b
meta #0: type 0x02 len 2
07
codec_capabilities 202CE99C dir 0x01
codec 0x06 cid 0x0000 vid 0x0000 count 4
data #0: type 0x01 len 1

data #1: type 0x02 len 1

data #2: type 0x03 len 4
010000
data #3: type 0x04 len 2
64
meta #0: type 0x02 len 2
07
codec_capabilities 202CE99C dir 0x01
codec 0x06 cid 0x0000 vid 0x0000 count 4
data #0: type 0x01 len 1

data #1: type 0x02 len 1

data #2: type 0x03 len 4
010000
data #3: type 0x04 len 2
5a
meta #0: type 0x02 len 2
07
codec_capabilities 202CE99C dir 0x01
codec 0x06 cid 0x0000 vid 0x0000 count 4
data #0: type 0x01 len 1

data #1: type 0x02 len 1

data #2: type 0x03 len 4
010000
data #3: type 0x04 len 2
78
meta #0: type 0x02 len 2
07
codec_capabilities 202CE99C dir 0x01
codec 0x06 cid 0x0000 vid 0x0000 count 4
data #0: type 0x01 len 1

data #1: type 0x02 len 1

data #2: type 0x03 len 4
010000
data #3: type 0x04 len 2
75
meta #0: type 0x02 len 2
07
codec_capabilities 202CE99C dir 0x01
codec 0x06 cid 0x0000 vid 0x0000 count 4
data #0: type 0x01 len 1

data #1: type 0x02 len 1

data #2: type 0x03 len 4
010000
data #3: type 0x04 len 2
9b
meta #0: type 0x02 len 2
07
Sink #0: ep 202C2F44
Sink discover complete
Discover sources complete: err 0
Created group
Configured stream 202C83DC
QoS set stream 202C83DC
Enabled stream 202C83DC
Started stream 202C83DC
Sending mock data with len 100
CCP: Placing call to remote with id 1 to skype:friend
Sending mock data with len 100
Sending mock data with len 100
CCP: Call terminated for id 1 with reason 6
Sending mock data with len 100
Sending mock data with len 100
Sending mock data with len 100
Sending mock data with len 100
Sending mock data with len 100
Sending mock data with len 100
Sending mock data with len 100
Sending mock data with len 100
Sending mock data with len 100
Sending mock data with len 100
Sending mock data with len 100
Sending mock data with len 100
Sending mock data with len 100
Sending mock data with len 100
Sending mock data with len 100
Sending mock data with len 100
Sending mock data with len 100
Sending mock data with len 100
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Procedures to run
1 this demo don't need any user input and will run automatically.

## Supported Boards
- [MIMXRT1170-EVKB](../../_boards/evkbmimxrt1170/edgefast_bluetooth_examples/tmap_central/example_board_readme.md)
- [MIMXRT1060-EVKC](../../_boards/evkcmimxrt1060/edgefast_bluetooth_examples/tmap_central/example_board_readme.md)
- [MIMXRT700-EVK](../../_boards/mimxrt700evk/edgefast_bluetooth_examples/tmap_central/example_board_readme.md)
