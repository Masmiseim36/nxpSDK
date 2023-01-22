Overview
========
Demo for showing how to use the Device Shadow library's API.

Before building the example application select Wi-Fi module macro in the app_config.h. (see #define WIFI_<SoC Name>_BOARD_<Module Name>).
For more information about Wi-Fi module connection see:
    readme_modules.txt
    Getting started guide on supported modules configuration:
    https://www.nxp.com/document/guide/getting-started-with-nxp-wi-fi-modules-using-i-mx-rt-platform:GS-WIFI-MODULES-IMXRT-PLATFORM



Toolchain supported
===================
- IAR embedded Workbench  9.32.1
- Keil MDK  5.37
- GCC ARM Embedded  10.3.1
- MCUXpresso  11.7.0

Hardware requirements
=====================
- Micro USB cable
- EVK-MIMXRT595 board
- Personal Computer
- USB/UART converter (3.3V logic level)


Board settings
==============
JP4: 1-2

Debug console UART is configured to use pins of J27, connect the board with PC by USB/UART converter:
- board UART RX (pin 1 on J27) - connect to TX pin on converter
- board UART TX (pin 2 on J27) - connect to RX pin on converter
- board GND (pin 7 on J29) - connect to GND pin on converter

Prepare the Demo
================
Before running the demo it is need to configure AWS IoT Console and update some of project files:

1.  Create AWS Account: https://console.aws.amazon.com/console/home

2.  Configure device in the AWS IoT Console base on this guide:

    Make note of example's "Thing name" and "REST API endpoint". These strings need to be set in the "aws_clientcredential.h".

    Example:
        static const char clientcredentialMQTT_BROKER_ENDPOINT[] = "abcdefgh123456.iot.us-west-2.amazonaws.com";
        #define clientcredentialIOT_THING_NAME "MyExample"

    In the next step you will get the "device certificate" and the "primary key". The device certificate and private key needs to be opened in text editor and its content copied into the "aws_clientcredential_keys.h".
    Or you can use the PEM-to-C-string.py (mcu-sdk-2.0\middleware\aws_iot\amazon-freertos\tools\certificate_configuration) to generate the "aws_clientcredential_keys.h".

    Example:
        #define keyCLIENT_CERTIFICATE_PEM "Paste client certificate here."

        Needs to be changed to:

        #define keyCLIENT_CERTIFICATE_PEM "-----BEGIN CERTIFICATE-----\n"\
        "MIIDWTCCAkGgAwIBAgIUfmv3zA+JULlMOxmz+upkAzhEkQ0wDQYJKoZIhvcNAQEL\n"\
        .
        .
        .
        "mepuT3lKmD0jZupsQ9vLQOA09rMjVMd0YPmI9ozvvWqLpjVvNTKVhsf/3slM\n"\
        "-----END CERTIFICATE-----\n"

    In the same way update the private key array.

    Files "aws_clientcredential.h" and "aws_clientcredential_keys.h" are placed in project folder.

3.  This demo needs Wi-Fi network with internet access.
    Update these macros in "aws_clientcredential.h" based on your Wi-Fi network configuration:
        #define clientcredentialWIFI_SSID       "Paste Wi-Fi SSID here."
        #define clientcredentialWIFI_PASSWORD   "Paste Wi-Fi password here."

4.  Open example's project and build it.

5.  Connect a USB cable between the PC host and the OpenSDA port on the target board.

6.  Download the program to the target board with the CMSIS-DAP or J-Link debugger.

7.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

8.  Open a serial terminal on PC for serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control

Running the demo
================
The log below shows the output of the demo in the terminal window. The log can be different based on your Wi-Fi network configuration.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
.
.
.
6 13045 [MQTT] [INFO] Creating a TLS connection to a2zcot8a2tqh6c-ats.iot.us-east-2.amazonaws.com:8883.
7 14120 [MQTT] [INFO] (Network connection 2022b20c) TLS handshake successful.
8 14120 [MQTT] [INFO] (Network connection 2022b20c) Connection to a2zcot8a2tqh6c-ats.iot.us-east-2.amazonaws.com established.
9 14120 [MQTT] [INFO] Creating an MQTT connection to the broker.
10 14316 [MQTT] [INFO] MQTT connection established with the broker.
11 14316 [MQTT] [INFO] Successfully connected to MQTT broker.
12 14316 [SHADOW_APP] [INFO] MQTT Agent is connected. Initializing shadow update task.
13 14316 [SHADOW_APP] [INFO] Sending subscribe request to agent for shadow topics.
14 14317 [SHADOW_DEV] [INFO] MQTT Agent is connected. Initializing shadow device task.
15 14318 [SHADOW_DEV] [INFO] Sending subscribe request to agent for shadow topics.
16 14518 [SHADOW_APP] [INFO] Received subscribe ack for shadow update topics.
17 14668 [SHADOW_DEV] [INFO] Successfully subscribed to shadow update topics.
18 14668 [SHADOW_DEV] [INFO] Publishing to /get message using client token 14668.
19 14668 [MQTT] [INFO] Publishing message to $aws/things/aws_rpr_michal/shadow/get.

20 14669 [SHADOW_DEV] [INFO] Successfully sent a publish message to /get topic.
21 14819 [MQTT] [INFO] Ack packet deserialized with result: MQTTSuccess.
22 14819 [MQTT] [INFO] State record updated. New state=MQTTPublishDone.
23 14869 [MQTT] [INFO] De-serialized incoming PUBLISH packet: DeserializerResult=MQTTSuccess.
24 14869 [MQTT] [INFO] State record updated. New state=MQTTPubAckSend.
25 14870 [MQTT] [INFO] Ignoring GET accepted response without a delta field.
26 14871 [SHADOW_DEV] [INFO] Received an accepted response for shadow GET request. 
27 54518 [SHADOW_APP] [INFO] Publishing to /update with following client token 54518.
28 54518 [MQTT] [INFO] Publishing message to $aws/things/aws_rpr_michal/shadow/update.

29 54769 [MQTT] [INFO] Ack packet deserialized with result: MQTTSuccess.
30 54769 [MQTT] [INFO] State record updated. New state=MQTTPublishDone.
31 54819 [MQTT] [INFO] De-serialized incoming PUBLISH packet: DeserializerResult=MQTTSuccess.
32 54819 [MQTT] [INFO] State record updated. New state=MQTTPubAckSend.
33 54819 [MQTT] [INFO] Received accepted response for update with token 54518. 
34 94820 [SHADOW_APP] [INFO] Publishing to /update with following client token 94820.
35 94820 [MQTT] [INFO] Publishing message to $aws/things/aws_rpr_michal/shadow/update.

36 95071 [MQTT] [INFO] Ack packet deserialized with result: MQTTSuccess.
37 95071 [MQTT] [INFO] State record updated. New state=MQTTPublishDone.
38 95121 [MQTT] [INFO] De-serialized incoming PUBLISH packet: DeserializerResult=MQTTSuccess.
39 95121 [MQTT] [INFO] State record updated. New state=MQTTPubAckSend.
40 95121 [MQTT] [INFO] Received accepted response for update with token 94820. 
41 95172 [MQTT] [INFO] De-serialized incoming PUBLISH packet: DeserializerResult=MQTTSuccess.
42 95173 [MQTT] [INFO] State record updated. New state=MQTTPubAckSend.
43 95173 [MQTT] [INFO] Received delta update with version 86.
44 95173 [MQTT] [INFO] Setting device state to 1.
45 95174 [SHADOW_DEV] [INFO] Publishing to /update with following client token 95173.
46 95174 [MQTT] [INFO] Publishing message to $aws/things/aws_rpr_michal/shadow/update.

47 95174 [SHADOW_DEV] [INFO] Successfully sent a publish message to /update topic.
48 95524 [MQTT] [INFO] De-serialized incoming PUBLISH packet: DeserializerResult=MQTTSuccess.
49 95524 [MQTT] [INFO] State record updated. New state=MQTTPubAckSend.
50 95524 [MQTT] [INFO] Received accepted response for update with token 95173. 
51 95525 [SHADOW_DEV] [INFO] Successfully received a shadow update accepted message from cloud. 
52 135123 [SHADOW_APP] [INFO] Publishing to /update with following client token 135123.
53 135123 [MQTT] [INFO] Publishing message to $aws/things/aws_rpr_michal/shadow/update.

54 135424 [MQTT] [INFO] Ack packet deserialized with result: MQTTSuccess.
55 135424 [MQTT] [INFO] State record updated. New state=MQTTPublishDone.
56 135475 [MQTT] [INFO] De-serialized incoming PUBLISH packet: DeserializerResult=MQTTSuccess.
57 135475 [MQTT] [INFO] State record updated. New state=MQTTPubAckSend.
58 135475 [MQTT] [INFO] Received accepted response for update with token 135123. 
59 135477 [MQTT] [INFO] De-serialized incoming PUBLISH packet: DeserializerResult=MQTTSuccess.
60 135477 [MQTT] [INFO] State record updated. New state=MQTTPubAckSend.
61 135477 [MQTT] [INFO] Received delta update with version 88.
62 135477 [MQTT] [INFO] Setting device state to 0.
63 135518 [SHADOW_DEV] [INFO] Publishing to /update with following client token 135518.
64 135518 [MQTT] [INFO] Publishing message to $aws/things/aws_rpr_michal/shadow/update.

65 135519 [SHADOW_DEV] [INFO] Successfully sent a publish message to /update topic.
66 135819 [MQTT] [INFO] De-serialized incoming PUBLISH packet: DeserializerResult=MQTTSuccess.
67 135819 [MQTT] [INFO] State record updated. New state=MQTTPubAckSend.
68 135820 [MQTT] [INFO] Received accepted response for update with token 135518. 
69 135820 [SHADOW_DEV] [INFO] Successfully received a shadow update accepted message from cloud. 
70 175477 [SHADOW_APP] [INFO] Publishing to /update with following client token 175477.
71 175477 [MQTT] [INFO] Publishing message to $aws/things/aws_rpr_michal/shadow/update.

72 175728 [MQTT] [INFO] Ack packet deserialized with result: MQTTSuccess.
73 175728 [MQTT] [INFO] State record updated. New state=MQTTPublishDone.
74 175828 [MQTT] [INFO] De-serialized incoming PUBLISH packet: DeserializerResult=MQTTSuccess.
75 175828 [MQTT] [INFO] State record updated. New state=MQTTPubAckSend.
76 175828 [MQTT] [INFO] Received accepted response for update with token 175477. 
77 175830 [MQTT] [INFO] De-serialized incoming PUBLISH packet: DeserializerResult=MQTTSuccess.
78 175830 [MQTT] [INFO] State record updated. New state=MQTTPubAckSend.
79 175830 [MQTT] [INFO] Received delta update with version 90.
80 175830 [MQTT] [INFO] Setting device state to 1.
81 175871 [SHADOW_DEV] [INFO] Publishing to /update with following client token 175871.
82 175871 [MQTT] [INFO] Publishing message to $aws/things/aws_rpr_michal/shadow/update.

83 175872 [SHADOW_DEV] [INFO] Successfully sent a publish message to /update topic.
84 176222 [MQTT] [INFO] De-serialized incoming PUBLISH packet: DeserializerResult=MQTTSuccess.
85 176223 [MQTT] [INFO] State record updated. New state=MQTTPubAckSend.
86 176223 [MQTT] [INFO] Received accepted response for update with token 175871. 
87 176224 [SHADOW_DEV] [INFO] Successfully received a shadow update accepted message from cloud. 
88 215830 [SHADOW_APP] [INFO] Publishing to /update with following client token 215830.
89 215830 [MQTT] [INFO] Publishing message to $aws/things/aws_rpr_michal/shadow/update.

90 216081 [MQTT] [INFO] Ack packet deserialized with result: MQTTSuccess.
91 216081 [MQTT] [INFO] State record updated. New state=MQTTPublishDone.
92 216131 [MQTT] [INFO] De-serialized incoming PUBLISH packet: DeserializerResult=MQTTSuccess.
93 216131 [MQTT] [INFO] State record updated. New state=MQTTPubAckSend.
94 216131 [MQTT] [INFO] Received delta update with version 92.
95 216131 [MQTT] [INFO] Setting device state to 0.
96 216132 [SHADOW_DEV] [INFO] Publishing to /update with following client token 216132.
97 216133 [MQTT] [INFO] Publishing message to $aws/things/aws_rpr_michal/shadow/update.
.
.
.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
