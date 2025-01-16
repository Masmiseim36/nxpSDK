# peripheral_ht

## Overview
Application demonstrating the BLE Peripheral role, except that this application specifically exposes the HT (Health Thermometer) GATT Service. Once a device connects it will generate dummy temperature values.

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
The demo does not require user interaction. The application will automatically start advertising the Health Thermometer Service and it will accept the first connection request it receives. If the peer subscribes to receive temperature indications, these will be sent every 1 second. The temperature readings are simulated with values between 20 and 25 degrees Celsius.

## Supported Boards
- [EVKB-IMXRT1050](../../_boards/evkbimxrt1050/edgefast_bluetooth_examples/peripheral_ht/example_board_readme.md)
- [MIMXRT1170-EVKB](../../_boards/evkbmimxrt1170/edgefast_bluetooth_examples/peripheral_ht/example_board_readme.md)
- [MIMXRT1060-EVKC](../../_boards/evkcmimxrt1060/edgefast_bluetooth_examples/peripheral_ht/example_board_readme.md)
- [MIMXRT1040-EVK](../../_boards/evkmimxrt1040/edgefast_bluetooth_examples/peripheral_ht/example_board_readme.md)
- [MIMXRT1180-EVK](../../_boards/evkmimxrt1180/edgefast_bluetooth_examples/peripheral_ht/example_board_readme.md)
- [EVK-MIMXRT595](../../_boards/evkmimxrt595/edgefast_bluetooth_examples/peripheral_ht/example_board_readme.md)
- [EVK-MIMXRT685](../../_boards/evkmimxrt685/edgefast_bluetooth_examples/peripheral_ht/example_board_readme.md)
- [FRDM-RW612](../../_boards/frdmrw612/edgefast_bluetooth_examples/peripheral_ht/example_board_readme.md)
- [MCX-N5XX-EVK](../../_boards/mcxn5xxevk/edgefast_bluetooth_examples/peripheral_ht/example_board_readme.md)
- [MCX-N9XX-EVK](../../_boards/mcxn9xxevk/edgefast_bluetooth_examples/peripheral_ht/example_board_readme.md)
- [MIMXRT685-AUD-EVK](../../_boards/mimxrt685audevk/edgefast_bluetooth_examples/peripheral_ht/example_board_readme.md)
- [MIMXRT700-EVK](../../_boards/mimxrt700evk/edgefast_bluetooth_examples/peripheral_ht/example_board_readme.md)
- [RD-RW612-BGA](../../_boards/rdrw612bga/edgefast_bluetooth_examples/peripheral_ht/example_board_readme.md)
