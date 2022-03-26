Overview
========
The simple Shadow lightbulb example to illustrate how client application and things communicate with the Shadow service.


Toolchain supported
===================
- IAR embedded Workbench  9.20.2
- Keil MDK  5.36
- GCC ARM Embedded  10.3.1
- MCUXpresso  11.5.0

Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT1170-EVK board
- Personal Computer
- Network cable RJ45 standard (Network with Internet access)

Board settings
==============
This example uses 1G port(J4) as default. If want to test 100M port(J3), please set the macro BOARD_NETWORK_USE_100M_ENET_PORT to 1.

Prepare the Demo
================
Before running the demo it is need to configure AWS IoT Console and update some of project files:

1.  Create AWS Account: https://console.aws.amazon.com/console/home

2.  Configure device in the AWS IoT Console base on this guide: https://docs.aws.amazon.com/iot/latest/developerguide/iot-sdk-setup.html

    Make note of example's "Thing name" and "REST API endpoint". These strings need to be set in the "aws_clientcredential.h".

    Example:
        static const char clientcredentialMQTT_BROKER_ENDPOINT[] = "abcdefgh123456.iot.us-west-2.amazonaws.com";
        #define clientcredentialIOT_THING_NAME "MyExample"

    In the next step you will get the "device certificate" and the "primary key". The device certificate and private key needs to be opened in text editor and its content copied into the "aws_clientcredential_keys.h".
    Or you can use the CertificateConfigurator.html (mcu-sdk-2.0\rtos\freertos\tools\certificate_configuration) to generate the "aws_clientcredential_keys.h".

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

3.  This demo doesn't need Wi-Fi network, you can leave the following macros from "aws_clientcredential.h" unmodified.
        #define clientcredentialWIFI_SSID       "Paste Wi-Fi SSID here."
        #define clientcredentialWIFI_PASSWORD   "Paste Wi-Fi password here."

4.  Make sure you have Internet access.

5.  Open example's project and build it.

6.  Connect a USB cable between the PC host and the OpenSDA port on the target board.

7.  Download the program to the target board with the CMSIS-DAP or J-Link debugger.

8.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

9.  Open a serial terminal on PC for serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control

Running the demo
================
The log below shows the output of the demo in the terminal window. The log can be different based on your local network configuration.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Initializing PHY...
0 221 [Tmr Svc] Write certificate...

1 2109 [Tmr Svc] Getting IP address from DHCP ...

2 5112 [Tmr Svc] IPv4 Address: 192.168.199.37

3 5113 [Tmr Svc] DHCP OK

4 5118 [iot_thread] [INFO ][DEMO][5116] ---------STARTING DEMO---------


5 5123 [iot_thread] [INFO ][INIT][5123] SDK successfully initialized.

6 5129 [iot_thread] [INFO ][DEMO][5129] Successfully initialized the demo. Network type for the demo: 4

7 5131 [iot_thread] [INFO] Create a TCP connection to a2zcot8a2tqh6c-ats.iot.us-east-2.amazonaws.com:8883.
8 15560 [iot_thread] [INFO] Packet received. ReceivedBytes=2.
9 15561 [iot_thread] [INFO] CONNACK session present bit not set.
10 15563 [iot_thread] [INFO] Connection accepted.
11 15565 [iot_thread] [INFO] Received MQTT CONNACK successfully from broker.
12 15567 [iot_thread] [INFO] MQTT connection established with the broker.
13 15569 [iot_thread] [INFO] MQTT connection successfully established with broker.


14 15573 [iot_thread] [INFO] A clean MQTT connection is established. Cleaning up all the stored outgoing publishes.


15 15588 [iot_thread] [INFO] SUBSCRIBE topic $aws/things/aws_demo/shadow/delete/accepted to broker.


16 15855 [iot_thread] [INFO] Packet received. ReceivedBytes=3.
17 15856 [iot_thread] [INFO] MQTT_PACKET_TYPE_SUBACK.


18 17676 [iot_thread] [INFO] SUBSCRIBE topic $aws/things/aws_demo/shadow/delete/rejected to broker.


19 17942 [iot_thread] [INFO] Packet received. ReceivedBytes=3.
20 17944 [iot_thread] [INFO] MQTT_PACKET_TYPE_SUBACK.


21 19755 [iot_thread] [INFO] the published payload: 
 
22 19768 [iot_thread] [INFO] PUBLISH sent for topic $aws/things/aws_demo/shadow/delete to broker with packet ID 3.


23 20010 [iot_thread] [INFO] Packet received. ReceivedBytes=2.
24 20011 [iot_thread] [INFO] Ack packet deserialized with result: MQTTSuccess.
25 20013 [iot_thread] [INFO] State record updated. New state=MQTTPublishDone.
26 20015 [iot_thread] [INFO] PUBACK received for packet id 3.


27 20019 [iot_thread] [INFO] Cleaned up outgoing publish packet with packet id 3.


28 20218 [iot_thread] [INFO] Packet received. ReceivedBytes=94.
29 20219 [iot_thread] [INFO] De-serialized incoming PUBLISH packet: DeserializerResult=MQTTSuccess.
30 20221 [iot_thread] [INFO] State record updated. New state=MQTTPubAckSend.
31 20223 [iot_thread] [INFO] pPublishInfo->pTopicName:$aws/things/aws_demo/shadow/delete/accepted.
32 20227 [iot_thread] [INFO] Received an MQTT incoming publish on /delete/accepted topic.
33 21855 [iot_thread] [INFO] UNSUBSCRIBE sent topic $aws/things/aws_demo/shadow/delete/accepted to broker.


34 22139 [iot_thread] [INFO] Packet received. ReceivedBytes=2.
35 22142 [iot_thread] [INFO] MQTT_PACKET_TYPE_UNSUBACK.


36 23962 [iot_thread] [INFO] UNSUBSCRIBE sent topic $aws/things/aws_demo/shadow/delete/rejected to broker.


37 24258 [iot_thread] [INFO] Packet received. ReceivedBytes=2.
38 24259 [iot_thread] [INFO] MQTT_PACKET_TYPE_UNSUBACK.


39 26081 [iot_thread] [INFO] SUBSCRIBE topic $aws/things/aws_demo/shadow/update/delta to broker.


40 26342 [iot_thread] [INFO] Packet received. ReceivedBytes=3.
41 26345 [iot_thread] [INFO] MQTT_PACKET_TYPE_SUBACK.


42 28165 [iot_thread] [INFO] SUBSCRIBE topic $aws/things/aws_demo/shadow/update/accepted to broker.


43 28464 [iot_thread] [INFO] Packet received. ReceivedBytes=3.
44 28465 [iot_thread] [INFO] MQTT_PACKET_TYPE_SUBACK.


45 30287 [iot_thread] [INFO] SUBSCRIBE topic $aws/things/aws_demo/shadow/update/rejected to broker.


46 30551 [iot_thread] [INFO] Packet received. ReceivedBytes=3.
47 30554 [iot_thread] [INFO] MQTT_PACKET_TYPE_SUBACK.


48 32369 [iot_thread] [INFO] Send desired power state with 1.
49 32372 [iot_thread] [INFO] the published payload:{"state":{"desired":{"powerOn":1}},"clientToken":"032370"} 
 
50 32397 [iot_thread] [INFO] PUBLISH sent for topic $aws/things/aws_demo/shadow/update to broker with packet ID 9.


51 32677 [iot_thread] [INFO] Packet received. ReceivedBytes=2.
52 32678 [iot_thread] [INFO] Ack packet deserialized with result: MQTTSuccess.
53 32680 [iot_thread] [INFO] State record updated. New state=MQTTPublishDone.
54 32682 [iot_thread] [INFO] PUBACK received for packet id 9.


55 32684 [iot_thread] [INFO] Cleaned up outgoing publish packet with packet id 9.


56 32910 [iot_thread] [INFO] Packet received. ReceivedBytes=184.
57 32913 [iot_thread] [INFO] De-serialized incoming PUBLISH packet: DeserializerResult=MQTTSuccess.
58 32915 [iot_thread] [INFO] State record updated. New state=MQTTPubAckSend.
59 32917 [iot_thread] [INFO] pPublishInfo->pTopicName:$aws/things/aws_demo/shadow/update/delta.
60 32919 [iot_thread] [INFO] /update/delta json payload:{"version":128,"timestamp":1623060389,"state":{"powerOn":1},"metadata":{"powerOn":{"timestamp":1623060389}},"clientToken":"032370"}.
61 32923 [iot_thread] [INFO] version: 128
62 32925 [iot_thread] [INFO] version:128, ulCurrentVersion:0 

63 32931 [iot_thread] [INFO] The new power on state newState:1, ulCurrentPowerOnState:0 

64 32970 [iot_thread] [INFO] Packet received. ReceivedBytes=211.
65 32972 [iot_thread] [INFO] De-serialized incoming PUBLISH packet: DeserializerResult=MQTTSuccess.
66 32974 [iot_thread] [INFO] State record updated. New state=MQTTPubAckSend.
67 32976 [iot_thread] [INFO] pPublishInfo->pTopicName:$aws/things/aws_demo/shadow/update/accepted.
68 32978 [iot_thread] [INFO] /update/accepted json payload:{"state":{"desired":{"powerOn":1}},"metadata":{"desired":{"powerOn":{"timestamp":1623060389}}},"version":128,"timestamp":1623060389,"clientToken":"032370"}.
69 32986 [iot_thread] [INFO] clientToken: 032370
70 32988 [iot_thread] [INFO] receivedToken:32370, clientToken:0 

71 32990 [iot_thread] [WARN] The received clientToken=32370 is not identical with the one=0 we sent 
72 34409 [iot_thread] [INFO] Report to the state change: 1
73 34412 [iot_thread] [INFO] the published payload:{"state":{"reported":{"powerOn":1}},"clientToken":"034410"} 
 
74 34437 [iot_thread] [INFO] PUBLISH sent for topic $aws/things/aws_demo/shadow/update to broker with packet ID 10.


75 34670 [iot_thread] [INFO] Packet received. ReceivedBytes=2.
76 34671 [iot_thread] [INFO] Ack packet deserialized with result: MQTTSuccess.
77 34673 [iot_thread] [INFO] State record updated. New state=MQTTPublishDone.
78 34675 [iot_thread] [INFO] PUBACK received for packet id 10.


79 34677 [iot_thread] [INFO] Cleaned up outgoing publish packet with packet id 10.


80 34882 [iot_thread] [INFO] Packet received. ReceivedBytes=213.
81 34883 [iot_thread] [INFO] De-serialized incoming PUBLISH packet: DeserializerResult=MQTTSuccess.
82 34887 [iot_thread] [INFO] State record updated. New state=MQTTPubAckSend.
83 34889 [iot_thread] [INFO] pPublishInfo->pTopicName:$aws/things/aws_demo/shadow/update/accepted.
84 34891 [iot_thread] [INFO] /update/accepted json payload:{"state":{"reported":{"powerOn":1}},"metadata":{"reported":{"powerOn":{"timestamp":1623060391}}},"version":129,"timestamp":1623060391,"clientToken":"034410"}.
85 34897 [iot_thread] [INFO] clientToken: 034410
86 34899 [iot_thread] [INFO] receivedToken:34410, clientToken:34410 

87 34903 [iot_thread] [INFO] Received response from the device shadow. Previously published update with clientToken=34410 has been accepted. 
88 36522 [iot_thread] [INFO] Start to unsubscribe shadow topics and disconnect from MQTT. 

89 36536 [iot_thread] [INFO] UNSUBSCRIBE sent topic $aws/things/aws_demo/shadow/update/delta to broker.


90 36798 [iot_thread] [INFO] Packet received. ReceivedBytes=2.
91 36799 [iot_thread] [INFO] MQTT_PACKET_TYPE_UNSUBACK.


92 38619 [iot_thread] [INFO] UNSUBSCRIBE sent topic $aws/things/aws_demo/shadow/update/accepted to broker.


93 38876 [iot_thread] [INFO] Packet received. ReceivedBytes=2.
94 38879 [iot_thread] [INFO] MQTT_PACKET_TYPE_UNSUBACK.


95 40700 [iot_thread] [INFO] UNSUBSCRIBE sent topic $aws/things/aws_demo/shadow/update/rejected to broker.


96 40961 [iot_thread] [INFO] Packet received. ReceivedBytes=2.
97 40962 [iot_thread] [INFO] MQTT_PACKET_TYPE_UNSUBACK.


98 42783 [iot_thread] [INFO] Disconnected from the broker.
99 42804 [iot_thread] [INFO] Demo iteration 1 is successful.
100 42807 [iot_thread] [INFO ][DEMO][42805] memory_metrics::freertos_heap::before::bytes::74912

101 42811 [iot_thread] [INFO ][DEMO][42809] memory_metrics::freertos_heap::after::bytes::25392

102 42815 [iot_thread] [INFO ][DEMO][42815] memory_metrics::demo_task_stack::before::bytes::5336

103 42821 [iot_thread] [INFO ][DEMO][42821] memory_metrics::demo_task_stack::after::bytes::3376

104 43823 [iot_thread] [INFO ][DEMO][43823] Demo completed successfully.

105 43829 [iot_thread] [INFO ][INIT][43829] SDK cleanup done.

106 43833 [iot_thread] [INFO ][DEMO][43831] -------DEMO FINISHED-------

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
