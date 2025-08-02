# umr2bms

## Overview
Application is working as a bridge to demonstrate a function that relay the UMS stream to BMR.

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
Bluetooth initialized

Copyright  2024  NXP

UMR2BMS>>
UMR To BMS.
UMR: Location successfully set
UMR: Supported contexts successfully set
UMR: Available contexts successfully set
UMR: Advertising successfully started
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


## Supported Boards
- [MIMXRT1170-EVKB](../../_boards/evkbmimxrt1170/edgefast_bluetooth_examples/broadcast_media_sender/example_board_readme.md)
- [MIMXRT1060-EVKC](../../_boards/evkcmimxrt1060/edgefast_bluetooth_examples/broadcast_media_sender/example_board_readme.md)
- [MIMXRT700-EVK](../../_boards/mimxrt700evk/edgefast_bluetooth_examples/broadcast_media_sender/example_board_readme.md)
