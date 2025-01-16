# central_pxm

## Overview
Application demonstrating very basic BLE Central role functionality by scanning for other BLE devices and establishing a connection to the first one with a strong enough signal.
Except that this application specifically looks for Proximity Reporter.

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
The application will automatically start scanning and will connect to the first advertiser who is advertising the Link Loss Service. If the connection is successful, the application performs service discovery to find the characteristics of the Link Loss Service, as well as additional services and characteristics specified by the Proximity Profile, such as Immediate Alert and Tx Power services.

If the Tx Power service and its characteristics have been discovered, the application will read the peer's Tx power and display it. Example output:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Read successful - Tx Power Level: 20
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

If the Immediate Alert service and its characteristics have been discovered, the application will continuously monitor the connection RSSI and will trigger or stop the Immediate Alert on the peer when the value is crossing a preset threshold in either direction.

After the mandatory Link Loss service is discovered, the application will write the Link Loss Alert Level on the peer as HIGH_ALERT. To trigger the Link Loss Alert on the peer, the connection will have to be timed out. The user can trigger this by simply resetting the board (press the RST button).

## Supported Boards
- [EVKB-IMXRT1050](../../_boards/evkbimxrt1050/edgefast_bluetooth_examples/central_pxm/example_board_readme.md)
- [MIMXRT1170-EVKB](../../_boards/evkbmimxrt1170/edgefast_bluetooth_examples/central_pxm/example_board_readme.md)
- [MIMXRT1060-EVKC](../../_boards/evkcmimxrt1060/edgefast_bluetooth_examples/central_pxm/example_board_readme.md)
- [MIMXRT1040-EVK](../../_boards/evkmimxrt1040/edgefast_bluetooth_examples/central_pxm/example_board_readme.md)
- [MIMXRT1180-EVK](../../_boards/evkmimxrt1180/edgefast_bluetooth_examples/central_pxm/example_board_readme.md)
- [EVK-MIMXRT595](../../_boards/evkmimxrt595/edgefast_bluetooth_examples/central_pxm/example_board_readme.md)
- [EVK-MIMXRT685](../../_boards/evkmimxrt685/edgefast_bluetooth_examples/central_pxm/example_board_readme.md)
- [FRDM-RW612](../../_boards/frdmrw612/edgefast_bluetooth_examples/central_pxm/example_board_readme.md)
- [MCX-N5XX-EVK](../../_boards/mcxn5xxevk/edgefast_bluetooth_examples/central_pxm/example_board_readme.md)
- [MCX-N9XX-EVK](../../_boards/mcxn9xxevk/edgefast_bluetooth_examples/central_pxm/example_board_readme.md)
- [MIMXRT685-AUD-EVK](../../_boards/mimxrt685audevk/edgefast_bluetooth_examples/central_pxm/example_board_readme.md)
- [MIMXRT700-EVK](../../_boards/mimxrt700evk/edgefast_bluetooth_examples/central_pxm/example_board_readme.md)
- [RD-RW612-BGA](../../_boards/rdrw612bga/edgefast_bluetooth_examples/central_pxm/example_board_readme.md)
