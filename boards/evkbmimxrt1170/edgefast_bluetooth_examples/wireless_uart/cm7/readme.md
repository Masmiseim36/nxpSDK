# wireless_uart

## Overview
The application implements a custom GATT based Wireless UART Profile that emulates UART over BLE.the application can work as central and peripheral at the same time. central and peripheral role can be switched by user button.
To test the service/profile the "IoT Toolbox" application can be used which is available for both Android and iOS.IoT Toolbox can be found on iTunes or Google playstore.

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
The demo require user interaction. The application will automatically start advertising the wirless uart Service afte reset, the The application can accepte at most 8 connection when work as peripheral.
The application will start scan and connect to the wirless uart Service automatically,after short prees the user button, the The application can connect at most 8 connection when work as cnentral.

after reset, the Serial port terminal log is:

BLE Wireless Uart demo start...
Bluetooth initialized
Advertising successfully started

then we can use "IoT Toolbox" or another wireless_uart example(use B to refer to) to test the current device. 
peripheral role test:
please open "IoT Toolbox", check the "Wireless UART" option, one device named "NXP_WU" will be found, click the "NXP_WU", the software will connect to the NXP_WU, please accept the pair request, or else there maybe pair fail. take android as example, please check the " message notification bar" to find the Pair request. 
after pair, data could be sent/receive in the toolbox.

central role test:
let B work as default state after reset. 
short press the user button, the example will work as central can automatically connect to any discovered wireless uart example.each time short press, the example will scan and connect to wirelss uart service if new device is found.

BLE Wireless Uart demo start...
Bluetooth initialized
Advertising successfully started
Scanning successfully started
[DEVICE]: 24:FC:E5:9F:EE:EB (public), AD evt type 3, AD data len 28, RSSI -92
[DEVICE]: 64:86:7F:5A:7C:7F (random), AD evt type 0, AD data len 23, RSSI -81
[DEVICE]: 64:86:7F:5A:7C:7F (random), AD evt type 4, AD data len 0, RSSI -80
[DEVICE]: 65:F2:7E:9A:AF:C7 (random), AD evt type 0, AD data len 19, RSSI -89
[DEVICE]: 65:F2:7E:9A:AF:C7 (random), AD evt type 4, AD data len 0, RSSI -89
[DEVICE]: 63:F2:B1:6A:FC:3D (random), AD evt type 0, AD data len 18, RSSI -80
[DEVICE]: 63:F2:B1:6A:FC:3D (random), AD evt type 4, AD data len 0, RSSI -80
[DEVICE]: 78:B3:AA:89:78:3B (random), AD evt type 0, AD data len 18, RSSI -80
[DEVICE]: 78:B3:AA:89:78:3B (random), AD evt type 4, AD data len 0, RSSI -79
[DEVICE]: 80:D2:1D:E8:2B:7E (public), AD evt type 0, AD data len 21, RSSI -43
Connected to 80:D2:1D:E8:2B:7E (public)
GATT MTU exchanged: 65
[ATTRIBUTE] handle 25
[ATTRIBUTE] handle 26
Security changed: 80:D2:1D:E8:2B:7E (public) level 2 (error 0)
Note:
the device address, AD event type data len, and RSSI are variable, it depend on all the bluetooth device in test environment.


send data 12345 in B device's Serial port terminal, then current device will print the following log. 
Data received (length 5): 12345  

send data 123 in current device's Serial port terminal, then B device will print the following log. 
Data received (length 5): 123

long press the user button, the example will work as peripheral again.

## Supported Boards
- [EVKB-IMXRT1050](../../_boards/evkbimxrt1050/edgefast_bluetooth_examples/wireless_uart/example_board_readme.md)
- [MIMXRT1170-EVKB](../../_boards/evkbmimxrt1170/edgefast_bluetooth_examples/wireless_uart/example_board_readme.md)
- [MIMXRT1060-EVKC](../../_boards/evkcmimxrt1060/edgefast_bluetooth_examples/wireless_uart/example_board_readme.md)
- [MIMXRT1040-EVK](../../_boards/evkmimxrt1040/edgefast_bluetooth_examples/wireless_uart/example_board_readme.md)
- [MIMXRT1180-EVK](../../_boards/evkmimxrt1180/edgefast_bluetooth_examples/wireless_uart/example_board_readme.md)
- [EVK-MIMXRT595](../../_boards/evkmimxrt595/edgefast_bluetooth_examples/wireless_uart/example_board_readme.md)
- [EVK-MIMXRT685](../../_boards/evkmimxrt685/edgefast_bluetooth_examples/wireless_uart/example_board_readme.md)
- [FRDM-RW612](../../_boards/frdmrw612/edgefast_bluetooth_examples/wireless_uart/example_board_readme.md)
- [MCX-N5XX-EVK](../../_boards/mcxn5xxevk/edgefast_bluetooth_examples/wireless_uart/example_board_readme.md)
- [MCX-N9XX-EVK](../../_boards/mcxn9xxevk/edgefast_bluetooth_examples/wireless_uart/example_board_readme.md)
- [MIMXRT685-AUD-EVK](../../_boards/mimxrt685audevk/edgefast_bluetooth_examples/wireless_uart/example_board_readme.md)
- [MIMXRT700-EVK](../../_boards/mimxrt700evk/edgefast_bluetooth_examples/wireless_uart/example_board_readme.md)
- [RD-RW612-BGA](../../_boards/rdrw612bga/edgefast_bluetooth_examples/wireless_uart/example_board_readme.md)
