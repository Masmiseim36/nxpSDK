# lwip_mqtt_enet_qos_freertos

## Overview

The lwip_mqtt demo application demonstrates MQTT client connecting to MQTT
broker via unsecured socket. The application obtains IP and DNS addresses from
DHCP and resolves broker's hostname if needed. Then it connects to the broker
and subscribes to the "lwip_topic/#" and "lwip_other/#" topics. Further it
publishes five messages to the "lwip_topic/100" which are also received back as
the board is subscribed to the "lwip_topic/#". Meanwhile it continues to receive
messages published to the subscribed topics from other clients.

## Supported Boards
- [MIMXRT1170-EVKB](../../../_boards/evkbmimxrt1170/lwip_examples/lwip_mqtt_enet_qos/freertos/example_board_readme.md)
