# lwip_mqtt_freertos

## Overview

The lwip_mqtt demo application demonstrates MQTT client connecting to MQTT broker via unsecured socket.
The application obtains IP and DNS addresses from DHCP and resolves broker's hostname if needed.
Then it connects to the broker and subscribes to the "lwip_topic/#" and "lwip_other/#" topics.
Further it publishes five messages to the "lwip_topic/100" which are also received back
as the board is subscribed to the "lwip_topic/#".
Meanwhile it continues to receive messages published to the subscribed topics from other clients.

## Prepare the Demo
1.  Make sure the MQTT broker pointed to by EXAMPLE_MQTT_SERVER_HOST and EXAMPLE_MQTT_SERVER_PORT definitions
    is up and accessible on your network via unsecured socket. A public MQTT testing server can be used
    or you can install a broker locally and modify the definitions mentioned above.
2.  Connect a USB cable between the PC host and the OpenSDA(or USB to Serial) USB port on the target board.
3.  Open a serial terminal on PC for OpenSDA serial(or USB to Serial) device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Insert the Ethernet Cable into the target board's RJ45 port and connect it to a router (or other DHCP server capable device).
5.  Download the program to the target board.
6.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

## Running the demo
When the demo runs, the log would be seen on the terminal like:

Initializing PHY...

************************************************
 MQTT client example
************************************************
Getting IP address from DHCP...

IPv4 Address     : 192.168.0.102
IPv4 Subnet mask : 255.255.255.0
IPv4 Gateway     : 192.168.0.100

Resolving "broker.hivemq.com"...
Connecting to MQTT broker at 18.185.216.165...
MQTT client "nxp_f50003c25757bd58aa6d0ce50102f020" connected.
Subscribing to the topic "lwip_topic/#" with QoS 0...
Subscribing to the topic "lwip_other/#" with QoS 1...
Subscribed to the topic "lwip_topic/#".
Subscribed to the topic "lwip_other/#".
Going to publish to the topic "lwip_topic/100"...
Published to the topic "lwip_topic/100".
Received 18 bytes from the topic "lwip_topic/100": "message from board"
Going to publish to the topic "lwip_topic/100"...
Published to the topic "lwip_topic/100".
Received 18 bytes from the topic "lwip_topic/100": "message from board"
Going to publish to the topic "lwip_topic/100"...
Published to the topic "lwip_topic/100".
Received 18 bytes from the topic "lwip_topic/100": "message from board"
Going to publish to the topic "lwip_topic/100"...
Published to the topic "lwip_topic/100".
Received 18 bytes from the topic "lwip_topic/100": "message from board"
Going to publish to the topic "lwip_topic/100"...
Published to the topic "lwip_topic/100".
Received 18 bytes from the topic "lwip_topic/100": "message from board"

## Supported Boards
- [EVKB-IMXRT1050](../../../_boards/evkbimxrt1050/lwip_examples/lwip_mqtt/freertos/example_board_readme.md)
- [MIMXRT1060-EVKB](../../../_boards/evkbmimxrt1060/lwip_examples/lwip_mqtt/freertos/example_board_readme.md)
- [MIMXRT1170-EVKB](../../../_boards/evkbmimxrt1170/lwip_examples/lwip_mqtt/freertos/example_board_readme.md)
- [MIMXRT1060-EVKC](../../../_boards/evkcmimxrt1060/lwip_examples/lwip_mqtt/freertos/example_board_readme.md)
- [EVK-MIMXRT1020](../../../_boards/evkmimxrt1020/lwip_examples/lwip_mqtt/freertos/example_board_readme.md)
- [MIMXRT1024-EVK](../../../_boards/evkmimxrt1024/lwip_examples/lwip_mqtt/freertos/example_board_readme.md)
- [MIMXRT1040-EVK](../../../_boards/evkmimxrt1040/lwip_examples/lwip_mqtt/freertos/example_board_readme.md)
- [EVK-MIMXRT1064](../../../_boards/evkmimxrt1064/lwip_examples/lwip_mqtt/freertos/example_board_readme.md)
- [MIMXRT1160-EVK](../../../_boards/evkmimxrt1160/lwip_examples/lwip_mqtt/freertos/example_board_readme.md)
- [MIMXRT1180-EVK](../../../_boards/evkmimxrt1180/lwip_examples/lwip_mqtt/freertos/example_board_readme.md)
- [FRDM-MCXN947](../../../_boards/frdmmcxn947/lwip_examples/lwip_mqtt/freertos/example_board_readme.md)
- [FRDM-RW612](../../../_boards/frdmrw612/lwip_examples/lwip_mqtt/freertos/example_board_readme.md)
- [LPCXpresso54628](../../../_boards/lpcxpresso54628/lwip_examples/lwip_mqtt/freertos/example_board_readme.md)
- [LPCXpresso54S018](../../../_boards/lpcxpresso54s018/lwip_examples/lwip_mqtt/freertos/example_board_readme.md)
- [LPCXpresso54S018M](../../../_boards/lpcxpresso54s018m/lwip_examples/lwip_mqtt/freertos/example_board_readme.md)
- [MCX-N5XX-EVK](../../../_boards/mcxn5xxevk/lwip_examples/lwip_mqtt/freertos/example_board_readme.md)
- [MCX-N9XX-EVK](../../../_boards/mcxn9xxevk/lwip_examples/lwip_mqtt/freertos/example_board_readme.md)
- [RD-RW612-BGA](../../../_boards/rdrw612bga/lwip_examples/lwip_mqtt/freertos/example_board_readme.md)
