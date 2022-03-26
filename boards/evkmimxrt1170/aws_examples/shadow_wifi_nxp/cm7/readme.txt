Overview
========
The simple Shadow lightbulb example to illustrate how client application and things communicate with the Shadow service.

Before building the example application select Wi-Fi module macro in the app_config.h. (see #define WIFI_<SoC Name>_BOARD_<Module Name>).
For more information about Wi-Fi module connection see:
    readme_modules.txt
    Getting started guide on supported modules configuration:
    https://www.nxp.com/document/guide/getting-started-with-nxp-wi-fi-modules-using-i-mx-rt-platform:GS-WIFI-MODULES-IMXRT-PLATFORM



Toolchain supported
===================
- IAR embedded Workbench  9.20.2
- Keil MDK  5.36
- GCC ARM Embedded  10.3.1
- MCUXpresso  11.5.0

Hardware requirements
=====================
- Micro USB cable
- MIMXRT1170-EVK board
- Personal Computer


Board settings
==============

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
0 147 [Tmr Svc] Write certificate...

1 165 [iot_thread] [INFO ][DEMO][165] ---------STARTING DEMO---------


2 167 [iot_thread] [INFO ][INIT][167] SDK successfully initialized.

MAC Address:  0:13:43:7F:9D:F7 
[net] Initialized TCP/IP networking stack

3 3479 [iot_thread] Connecting to nxp .....

4 11382 [wlcmgr] Connected to with IP = [192.168.199.175]

5 11429 [iot_thread] [INFO ][DEMO][11429] Successfully initialized the demo. Network type for the demo: 1

6 11429 [iot_thread] [INFO] Create a TCP connection to a2zcot8a2tqh6c-ats.iot.us-east-2.amazonaws.com:8883.
7 14129 [iot_thread] [INFO] Packet received. ReceivedBytes=2.
8 14129 [iot_thread] [INFO] CONNACK session present bit not set.
9 14129 [iot_thread] [INFO] Connection accepted.
10 14129 [iot_thread] [INFO] Received MQTT CONNACK successfully from broker.
11 14129 [iot_thread] [INFO] MQTT connection established with the broker.
12 14129 [iot_thread] [INFO] MQTT connection successfully established with broker.


13 14129 [iot_thread] [INFO] A clean MQTT connection is established. Cleaning up all the stored outgoing publishes.


14 14131 [iot_thread] [INFO] SUBSCRIBE topic $aws/things/aws_demo/shadow/delete/accepted to broker.


15 14417 [iot_thread] [INFO] Packet received. ReceivedBytes=3.
16 14419 [iot_thread] [INFO] MQTT_PACKET_TYPE_SUBACK.


17 16228 [iot_thread] [INFO] SUBSCRIBE topic $aws/things/aws_demo/shadow/delete/rejected to broker.


18 16483 [iot_thread] [INFO] Packet received. ReceivedBytes=3.
19 16483 [iot_thread] [INFO] MQTT_PACKET_TYPE_SUBACK.


20 18292 [iot_thread] [INFO] the published payload: 
 
21 18292 [iot_thread] [INFO] PUBLISH sent for topic $aws/things/aws_demo/shadow/delete to broker with packet ID 3.


22 18529 [iot_thread] [INFO] Packet received. ReceivedBytes=2.
23 18529 [iot_thread] [INFO] Ack packet deserialized with result: MQTTSuccess.
24 18529 [iot_thread] [INFO] State record updated. New state=MQTTPublishDone.
25 18529 [iot_thread] [INFO] PUBACK received for packet id 3.


26 18529 [iot_thread] [INFO] Cleaned up outgoing publish packet with packet id 3.


27 18769 [iot_thread] [INFO] Packet received. ReceivedBytes=90.
28 18769 [iot_thread] [INFO] De-serialized incoming PUBLISH packet: DeserializerResult=MQTTSuccess.
29 18769 [iot_thread] [INFO] State record updated. New state=MQTTPubAckSend.
30 18769 [iot_thread] [INFO] pPublishInfo->pTopicName:$aws/things/aws_demo/shadow/delete/accepted.
31 18769 [iot_thread] [INFO] Received an MQTT incoming publish on /delete/accepted topic.
32 20379 [iot_thread] [INFO] UNSUBSCRIBE sent topic $aws/things/aws_demo/shadow/delete/accepted to broker.


33 20612 [iot_thread] [INFO] Packet received. ReceivedBytes=2.
34 20612 [iot_thread] [INFO] MQTT_PACKET_TYPE_UNSUBACK.


35 22421 [iot_thread] [INFO] UNSUBSCRIBE sent topic $aws/things/aws_demo/shadow/delete/rejected to broker.


36 22810 [iot_thread] [INFO] Packet received. ReceivedBytes=2.
37 22810 [iot_thread] [INFO] MQTT_PACKET_TYPE_UNSUBACK.


38 24619 [iot_thread] [INFO] SUBSCRIBE topic $aws/things/aws_demo/shadow/update/delta to broker.


39 24897 [iot_thread] [INFO] Packet received. ReceivedBytes=3.
40 24897 [iot_thread] [INFO] MQTT_PACKET_TYPE_SUBACK.


41 26706 [iot_thread] [INFO] SUBSCRIBE topic $aws/things/aws_demo/shadow/update/accepted to broker.


42 26962 [iot_thread] [INFO] Packet received. ReceivedBytes=3.
43 26962 [iot_thread] [INFO] MQTT_PACKET_TYPE_SUBACK.


44 28771 [iot_thread] [INFO] SUBSCRIBE topic $aws/things/aws_demo/shadow/update/rejected to broker.


45 29050 [iot_thread] [INFO] Packet received. ReceivedBytes=3.
46 29050 [iot_thread] [INFO] MQTT_PACKET_TYPE_SUBACK.


47 30859 [iot_thread] [INFO] Send desired power state with 1.
48 30859 [iot_thread] [INFO] the published payload:{"state":{"desired":{"powerOn":1}},"clientToken":"030859"} 
 
49 30859 [iot_thread] [INFO] PUBLISH sent for topic $aws/things/aws_demo/shadow/update to broker with packet ID 9.


50 31096 [iot_thread] [INFO] Packet received. ReceivedBytes=2.
51 31097 [iot_thread] [INFO] Ack packet deserialized with result: MQTTSuccess.
52 31097 [iot_thread] [INFO] State record updated. New state=MQTTPublishDone.
53 31097 [iot_thread] [INFO] PUBACK received for packet id 9.


54 31097 [iot_thread] [INFO] Cleaned up outgoing publish packet with packet id 9.


55 31305 [iot_thread] [INFO] Packet received. ReceivedBytes=180.
56 31305 [iot_thread] [INFO] De-serialized incoming PUBLISH packet: DeserializerResult=MQTTSuccess.
57 31305 [iot_thread] [INFO] State record updated. New state=MQTTPubAckSend.
58 31305 [iot_thread] [INFO] pPublishInfo->pTopicName:$aws/things/aws_demo/shadow/update/delta.
59 31305 [iot_thread] [INFO] /update/delta json payload:{"version":264,"timestamp":1623070061,"state":{"powerOn":1},"metadata":{"powerOn":{"timestamp":1623070061}},"clientToken":"030859"}.
60 31305 [iot_thread] [INFO] version: 264
61 31305 [iot_thread] [INFO] version:264, ulCurrentVersion:0 

62 31305 [iot_thread] [INFO] The new power on state newState:1, ulCurrentPowerOnState:0 

63 31307 [iot_thread] [INFO] Packet received. ReceivedBytes=207.
64 31307 [iot_thread] [INFO] De-serialized incoming PUBLISH packet: DeserializerResult=MQTTSuccess.
65 31307 [iot_thread] [INFO] State record updated. New state=MQTTPubAckSend.
66 31307 [iot_thread] [INFO] pPublishInfo->pTopicName:$aws/things/aws_demo/shadow/update/accepted.
67 31307 [iot_thread] [INFO] /update/accepted json payload:{"state":{"desired":{"powerOn":1}},"metadata":{"desired":{"powerOn":{"timestamp":1623070061}}},"version":264,"timestamp":1623070061,"clientToken":"030859"}.
68 31307 [iot_thread] [INFO] clientToken: 030859
69 31307 [iot_thread] [INFO] receivedToken:30859, clientToken:0 

70 31307 [iot_thread] [WARN] The received clientToken=30859 is not identical with the one=0 we sent 
71 32915 [iot_thread] [INFO] Report to the state change: 1
72 32915 [iot_thread] [INFO] the published payload:{"state":{"reported":{"powerOn":1}},"clientToken":"032915"} 
 
73 32915 [iot_thread] [INFO] PUBLISH sent for topic $aws/things/aws_demo/shadow/update to broker with packet ID 10.


74 33163 [iot_thread] [INFO] Packet received. ReceivedBytes=2.
75 33163 [iot_thread] [INFO] Ack packet deserialized with result: MQTTSuccess.
76 33163 [iot_thread] [INFO] State record updated. New state=MQTTPublishDone.
77 33163 [iot_thread] [INFO] PUBACK received for packet id 10.


78 33163 [iot_thread] [INFO] Cleaned up outgoing publish packet with packet id 10.


79 33405 [iot_thread] [INFO] Packet received. ReceivedBytes=209.
80 33405 [iot_thread] [INFO] De-serialized incoming PUBLISH packet: DeserializerResult=MQTTSuccess.
81 33405 [iot_thread] [INFO] State record updated. New state=MQTTPubAckSend.
82 33405 [iot_thread] [INFO] pPublishInfo->pTopicName:$aws/things/aws_demo/shadow/update/accepted.
83 33405 [iot_thread] [INFO] /update/accepted json payload:{"state":{"reported":{"powerOn":1}},"metadata":{"reported":{"powerOn":{"timestamp":1623070063}}},"version":265,"timestamp":1623070063,"clientToken":"032915"}.
84 33405 [iot_thread] [INFO] clientToken: 032915
85 33405 [iot_thread] [INFO] receivedToken:32915, clientToken:32915 

86 33405 [iot_thread] [INFO] Received response from the device shadow. Previously published update with clientToken=32915 has been accepted. 
87 35015 [iot_thread] [INFO] Start to unsubscribe shadow topics and disconnect from MQTT. 

88 35015 [iot_thread] [INFO] UNSUBSCRIBE sent topic $aws/things/aws_demo/shadow/update/delta to broker.


89 35256 [iot_thread] [INFO] Packet received. ReceivedBytes=2.
90 35256 [iot_thread] [INFO] MQTT_PACKET_TYPE_UNSUBACK.


91 37065 [iot_thread] [INFO] UNSUBSCRIBE sent topic $aws/things/aws_demo/shadow/update/accepted to broker.


92 37327 [iot_thread] [INFO] Packet received. ReceivedBytes=2.
93 37327 [iot_thread] [INFO] MQTT_PACKET_TYPE_UNSUBACK.


94 39136 [iot_thread] [INFO] UNSUBSCRIBE sent topic $aws/things/aws_demo/shadow/update/rejected to broker.


95 39412 [iot_thread] [INFO] Packet received. ReceivedBytes=2.
96 39412 [iot_thread] [INFO] MQTT_PACKET_TYPE_UNSUBACK.


97 41221 [iot_thread] [INFO] Disconnected from the broker.
98 41223 [iot_thread] [INFO] Demo iteration 1 is successful.
99 41223 [iot_thread] [INFO ][DEMO][41223] memory_metrics::freertos_heap::before::bytes::89440

100 41223 [iot_thread] [INFO ][DEMO][41223] memory_metrics::freertos_heap::after::bytes::16536

101 41223 [iot_thread] [INFO ][DEMO][41223] memory_metrics::demo_task_stack::before::bytes::5336

102 41223 [iot_thread] [INFO ][DEMO][41223] memory_metrics::demo_task_stack::after::bytes::3272

103 42223 [iot_thread] [INFO ][DEMO][42223] Demo completed successfully.

104 42225 [wlcmgr] Dis-connected

105 42273 [iot_thread] [INFO ][INIT][42273] SDK cleanup done.

106 42273 [iot_thread] [INFO ][DEMO][42273] -------DEMO FINISHED-------

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
