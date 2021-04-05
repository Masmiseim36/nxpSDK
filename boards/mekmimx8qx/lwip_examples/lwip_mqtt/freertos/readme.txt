Overview
========

The lwip_mqtt demo application demonstrates MQTT client connecting to MQTT broker via unsecured socket.
The application obtains IP and DNS addresses from DHCP and resolves broker's hostname if needed.
Then it connects to the broker and subscribes to the "lwip_topic/#" and "lwip_other/#" topics.
Further it publishes five messages to the "lwip_topic/100" which are also received back
as the board is subscribed to the "lwip_topic/#".
Meanwhile it continues to receive messages published to the subscribed topics from other clients.


Toolchain supported
===================
- GCC ARM Embedded  9.3.1
- IAR embedded Workbench  8.50.9

Hardware requirements
=====================
- Micro USB cable
- MIMX8QX MEK board
- J-Link Debug Probe
- 12V power supply
- Network cable RJ45 standard
- Personal Computer

Board settings
==============
No special setting needed.

#### Please note this application can't support running with Linux BSP! ####
This example aims to show the basic usage of the IP's function, some of the used Pads/Resources are also used by Cortex-A core.

Prepare the Demo
================
1.  Connect 12V power supply and J-Link Debug Probe to the board, switch SW3 to power on the board.
2.  Connect a micro USB cable between the host PC and the J11 USB port on the cpu board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Insert Cable to Ethernet RJ45 port and connect it to a router (or other DHCP server capable device).
5.  Download the program to the target board.
6.  Launch the debugger in your IDE to begin running the example.

Running the demo
================
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
