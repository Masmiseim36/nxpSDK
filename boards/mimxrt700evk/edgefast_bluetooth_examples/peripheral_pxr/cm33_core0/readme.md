# peripheral_pxr

## Overview
Application demonstrating the BLE Peripheral role, except that this application specifically exposes the Proximity Reporter (including LLS, IAS, and TPS) GATT Service.

## Prepare the Demo

1.  Open example's project and build it.

2.  Connect a USB cable between the PC host and the OpenSDA USB port on the target board.

3.  Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control

4.  Download the program to the target board.

5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

## Running the demo
The demo does not require user interaction. The application will automatically start advertising the Link Loss Service and it will accept the first connection request it receives. The application is then ready to process operations from the peer.

The application will initially set the default levels for the Link Loss Alert and the Immediate Alert. Example output:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Locally setting Immediate Alert...
ALERT: OFF
Locally setting Link Loss Alert Level to OFF
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The Proximity Monitor peer will trigger or stop the Immediate Alert on the application depending on the connection RSSI. Example output:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Monitor is setting Immediate Alert...
ALERT: HIGH
Monitor is setting Immediate Alert...
ALERT: OFF
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

If the connection with the Proximity Monitor is timed out, the Link Loss Alert will be triggered with the level previously set by the Monitor. Example output:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Link Loss Alert Triggered...
ALERT: HIGH
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Supported Boards
- [EVKB-IMXRT1050](../../_boards/evkbimxrt1050/edgefast_bluetooth_examples/peripheral_pxr/example_board_readme.md)
- [MIMXRT1170-EVKB](../../_boards/evkbmimxrt1170/edgefast_bluetooth_examples/peripheral_pxr/example_board_readme.md)
- [MIMXRT1060-EVKC](../../_boards/evkcmimxrt1060/edgefast_bluetooth_examples/peripheral_pxr/example_board_readme.md)
- [MIMXRT1040-EVK](../../_boards/evkmimxrt1040/edgefast_bluetooth_examples/peripheral_pxr/example_board_readme.md)
- [MIMXRT1180-EVK](../../_boards/evkmimxrt1180/edgefast_bluetooth_examples/peripheral_pxr/example_board_readme.md)
- [EVK-MIMXRT595](../../_boards/evkmimxrt595/edgefast_bluetooth_examples/peripheral_pxr/example_board_readme.md)
- [EVK-MIMXRT685](../../_boards/evkmimxrt685/edgefast_bluetooth_examples/peripheral_pxr/example_board_readme.md)
- [FRDM-RW612](../../_boards/frdmrw612/edgefast_bluetooth_examples/peripheral_pxr/example_board_readme.md)
- [MCX-N5XX-EVK](../../_boards/mcxn5xxevk/edgefast_bluetooth_examples/peripheral_pxr/example_board_readme.md)
- [MCX-N9XX-EVK](../../_boards/mcxn9xxevk/edgefast_bluetooth_examples/peripheral_pxr/example_board_readme.md)
- [MIMXRT685-AUD-EVK](../../_boards/mimxrt685audevk/edgefast_bluetooth_examples/peripheral_pxr/example_board_readme.md)
- [MIMXRT700-EVK](../../_boards/mimxrt700evk/edgefast_bluetooth_examples/peripheral_pxr/example_board_readme.md)
- [RD-RW612-BGA](../../_boards/rdrw612bga/edgefast_bluetooth_examples/peripheral_pxr/example_board_readme.md)
