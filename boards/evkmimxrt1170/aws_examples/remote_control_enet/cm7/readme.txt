Overview
========
This example demonstrates how the board can be controlled by Android mobile application with usage the AWS IoT.
User LEDs on the board can be controlled and also an action can be invoked to read data from accelerometer and report it to AWS IoT device shadow.
Please take look into section "Board settings" if there are some board limitations to control LEDs or read data from accelerometer.


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

3.  Open example's project and build it.

4.  Connect a USB cable between the PC host and the OpenSDA USB port on the target board.

5.  Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control

6.  Connect the board's RJ45 to network with Internet access (IP address to the board is assigned by the DHCP server). Make sure the connection on port 8883 is not blocked.

7.  Download the program to the target board.

8.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.


Prepare the Android application
The Android application requires Cognito service to authorize to AWS IoT in order to access device shadows. Use Amazon Cognito to create a new identity pool:

1.  In the Amazon Cognito Console https://console.aws.amazon.com/cognito/ select "Manage Federated Identities" and "Create new identity pool".

2.  Name your pool and ensure "Enable access to unauthenticated identities" is checked. This allows the sample application to assume the unauthenticated role associated with this identity pool.
    Note: to keep this example simple it makes use of unauthenticated users in the identity pool. This can be used for getting started and prototypes but unauthenticated users should typically only be given read-only permissions in production applications. More information on Cognito identity pools including the Cognito developer guide can be found here: http://aws.amazon.com/cognito/.

3.  To obtain the Pool ID constant, select "Edit identity pool" and copy Identity pool ID (it will look like <REGION>:<ID>). This Identity pool ID (<COGNITO POOL ID>) will be used in the application (policy and configuration file).

4.  To obtain Account ID, select account name in webpage menu bar and select "My account" from drop down menu. Make note of "Account ID" under "Account Settings".

5. As part of creating the identity pool Cognito will setup two roles in Identity and Access Management (IAM) https://console.aws.amazon.com/iam/home#roles. These will be named something similar to: "Cognito_PoolNameAuth_Role" and "Cognito_PoolNameUnauth_Role".
Create policy to be attached into "Cognito_PoolNameUnauth_Role" though "Policies" menu, selecting "Create policy", "Create Your Own Policy" and copying example policy below into "Policy Document" field and naming it for example "<THING NAME>Policy". Replace <REGION>, <ACCOUNT ID> and <THING NAME> with your respective values. This policy allows the application to get and update the two thing shadows used in this sample.

    {
        "Version": "2012-10-17",
        "Statement": [
            {
                "Effect": "Allow",
                "Action": [
                    "iot:Connect"
                ],
                "Resource": [
                    "*"
                ]
            },
            {
                "Effect": "Allow",
                "Action": [
                    "iot:Publish"
                ],
                "Resource": [
                    "arn:aws:iot:<REGION>:<ACCOUNT ID>:topic/$aws/things/<THING NAME>/shadow/update",
                    "arn:aws:iot:<REGION>:<ACCOUNT ID>:topic/$aws/things/<THING NAME>/shadow/get"
                ]
            },
            {
                "Effect": "Allow",
                "Action": [
                    "iot:Subscribe",
                    "iot:Receive"
                ],
                "Resource": [
                    "*"
                ]
            }
        ]
    }

6.  Newly created policy now needs to be attached to the unauthenticated role which has permissions to access the required AWS IoT APIs by opening "Cognito_PoolNameUnauth_Role" under "Roles" menu. Then in "Permissions" tab select "Attach policy" to view list of all AWS policies where example's policy "<THING NAME>Policy" needs to be selected though checking its checkbox and clicking on "Attach policy" button.

    More information on AWS IAM roles and policies can be found here: http://docs.aws.amazon.com/IAM/latest/UserGuide/access_policies_manage.html
    More information on AWS IoT policies can be found here: http://docs.aws.amazon.com/iot/latest/developerguide/authorization.html

7.  Prepare "AwsRemoteControlPreferences.properties" file with yours AWS credentials. It's structure looks like this:

    customer_specific_endpoint=<REST API ENDPOINT>
    cognito_pool_id=<COGNITO POOL ID>
    thing_name=<THING NAME>
    region=<REGION>

    Then move properties file into your Android device (application will ask for properties file though file browser dialog during first run).

8.  To run Android application do either:
    a) install and run pre-build apk on Android device (<SDK_Repository>\boards\<board_name>\aws_examples\remote_control\android\AwsRemoteControl.apk)
    b) open project in Android Studio, build it, attach Android device and Run application

    Application requires at least Android version 5.1 (Android SDK 22).

    Then in both cases when asked select AwsRemoteControlPreferences.properties file with AWS IoT preferences. Then application will establish MQTT connection to AWS server, download last state of thing's shadow and will be ready for user input.


Running the demo
================
The log below shows the output of the demo in the terminal window. The log can be different based on the actions, which you have done in the Android application.

Android application displays Accelerometer data for all three axis and status of LEDs which is split into Desired and Reported section. Desired value is value wanted by user and Reported value is actual value of LEDs on device.

- When you turn on/off some of RGB LED in the Android application, the change should be visible on RGB LED on the board.
- When you refresh accelerometer data on Android application, you should see changes in the accelerometer data.

Every mentioned action takes approximately 1-3 seconds.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Initializing PHY...
0 254 [Tmr Svc] Write certificate...

1 2369 [Tmr Svc] Getting IP address from DHCP ...

2 5369 [Tmr Svc] IPv4 Address: 10.42.0.198

3 5369 [Tmr Svc] DHCP OK

4 5370 [iot_thread] [INFO ][DEMO][5370] ---------STARTING DEMO---------


5 5372 [iot_thread] [INFO ][INIT][5372] SDK successfully initialized.

6 5372 [iot_thread] [INFO ][DEMO][5372] Successfully initialized the demo. Network type for the demo: 4

7 5372 [iot_thread] [INFO] Create a TCP connection to a2zcot8a2tqh6c-ats.iot.us-east-2.amazonaws.com:8883.
8 10736 [iot_thread] [INFO] Packet received. ReceivedBytes=2.
9 10736 [iot_thread] [INFO] CONNACK session present bit not set.
10 10736 [iot_thread] [INFO] Connection accepted.
11 10736 [iot_thread] [INFO] Received MQTT CONNACK successfully from broker.
12 10736 [iot_thread] [INFO] MQTT connection established with the broker.
13 10738 [iot_thread] [INFO] MQTT connection successfully established with broker.


14 10738 [iot_thread] [INFO] A clean MQTT connection is established. Cleaning up all the stored outgoing publishes.


15 10739 [iot_thread] [INFO] SUBSCRIBE topic $aws/things/aws_demo/shadow/delete/accepted to broker.


16 10888 [iot_thread] [INFO] Packet received. ReceivedBytes=3.
17 10888 [iot_thread] [INFO] MQTT_PACKET_TYPE_SUBACK.


18 12898 [iot_thread] [INFO] SUBSCRIBE topic $aws/things/aws_demo/shadow/delete/rejected to broker.


19 13052 [iot_thread] [INFO] Packet received. ReceivedBytes=3.
20 13052 [iot_thread] [INFO] MQTT_PACKET_TYPE_SUBACK.


21 15062 [iot_thread] [INFO] the published payload: 
 
22 15063 [iot_thread] [INFO] PUBLISH sent for topic $aws/things/aws_demo/shadow/delete to broker with packet ID 3.


23 15214 [iot_thread] [INFO] Packet received. ReceivedBytes=2.
24 15214 [iot_thread] [INFO] Ack packet deserialized with result: MQTTSuccess.
25 15216 [iot_thread] [INFO] State record updated. New state=MQTTPublishDone.
26 15216 [iot_thread] [INFO] PUBACK received for packet id 3.


27 15216 [iot_thread] [INFO] Cleaned up outgoing publish packet with packet id 3.


28 15251 [iot_thread] [INFO] Packet received. ReceivedBytes=92.
29 15251 [iot_thread] [INFO] De-serialized incoming PUBLISH packet: DeserializerResult=MQTTSuccess.
30 15251 [iot_thread] [INFO] State record updated. New state=MQTTPubAckSend.
31 15253 [iot_thread] [INFO] pPublishInfo->pTopicName:$aws/things/aws_demo/shadow/delete/accepted.
32 15253 [iot_thread] [INFO] Received an MQTT incoming publish on /delete/accepted topic.
33 17063 [iot_thread] [INFO] UNSUBSCRIBE sent topic $aws/things/aws_demo/shadow/delete/accepted to broker.


34 17268 [iot_thread] [INFO] Packet received. ReceivedBytes=2.
35 17268 [iot_thread] [INFO] MQTT_PACKET_TYPE_UNSUBACK.


36 19077 [iot_thread] [INFO] UNSUBSCRIBE sent topic $aws/things/aws_demo/shadow/delete/rejected to broker.


37 19239 [iot_thread] [INFO] Packet received. ReceivedBytes=2.
38 19239 [iot_thread] [INFO] MQTT_PACKET_TYPE_UNSUBACK.


39 21249 [iot_thread] [INFO] SUBSCRIBE topic $aws/things/aws_demo/shadow/update/delta to broker.


40 21528 [iot_thread] [INFO] Packet received. ReceivedBytes=3.
41 21529 [iot_thread] [INFO] MQTT_PACKET_TYPE_SUBACK.


42 23338 [iot_thread] [INFO] the published payload:{"state":{"desired":{"LEDstate":0},"reported":{"LEDstate":0,"accel":{"x":0,"y":0,"z":0},"LEDinfo":{"isRgbLed":true,"colors":["red", "green", "blue"]}}},"clientToken": "token-23338"} 
 
43 23339 [iot_thread] [INFO] PUBLISH sent for topic $aws/things/aws_demo/shadow/update to broker with packet ID 7.


44 23621 [iot_thread] [INFO] Packet received. ReceivedBytes=2.
45 23621 [iot_thread] [INFO] Ack packet deserialized with result: MQTTSuccess.
46 23621 [iot_thread] [INFO] State record updated. New state=MQTTPublishDone.
47 23621 [iot_thread] [INFO] PUBACK received for packet id 7.


48 23621 [iot_thread] [INFO] Cleaned up outgoing publish packet with packet id 7.


49 25430 [iot_thread] [INFO] AWS Remote Control Demo initialized.
50 25430 [iot_thread] [INFO] Use mobile application to control the remote device.
.
.
.
51 38952 [iot_thread] [INFO] Packet received. ReceivedBytes=161.
52 38952 [iot_thread] [INFO] De-serialized incoming PUBLISH packet: DeserializerResult=MQTTSuccess.
53 38952 [iot_thread] [INFO] State record updated. New state=MQTTPubAckSend.
54 38952 [iot_thread] [INFO] pPublishInfo->pTopicName:$aws/things/aws_demo/shadow/update/delta.
55 38952 [iot_thread] [INFO] /update/delta json payload:{"version":1043,"timestamp":1623423424,"state":{"LEDstate":1},"metadata":{"LEDstate":{"timestamp":1623423424}}}.
56 38954 [iot_thread] [INFO] version: 1043
57 38954 [iot_thread] [INFO] version:1043, ulCurrentVersion:0 

58 39156 [iot_thread] [INFO] Turn on LED Red
59 39156 [iot_thread] [INFO] the published payload:{"state":{"reported":{"LEDstate":1}},"clientToken": "token-39156"} 
 
60 39157 [iot_thread] [INFO] PUBLISH sent for topic $aws/things/aws_demo/shadow/update to broker with packet ID 8.


61 39449 [iot_thread] [INFO] Packet received. ReceivedBytes=2.
62 39449 [iot_thread] [INFO] Ack packet deserialized with result: MQTTSuccess.
63 39449 [iot_thread] [INFO] State record updated. New state=MQTTPublishDone.
64 39449 [iot_thread] [INFO] PUBACK received for packet id 8.


65 39449 [iot_thread] [INFO] Cleaned up outgoing publish packet with packet id 8.


66 41258 [iot_thread] [INFO] Successfully performed update.
.
.
.
67 45013 [iot_thread] [INFO] Packet received. ReceivedBytes=161.
68 45013 [iot_thread] [INFO] De-serialized incoming PUBLISH packet: DeserializerResult=MQTTSuccess.
69 45013 [iot_thread] [INFO] State record updated. New state=MQTTPubAckSend.
70 45013 [iot_thread] [INFO] pPublishInfo->pTopicName:$aws/things/aws_demo/shadow/update/delta.
71 45013 [iot_thread] [INFO] /update/delta json payload:{"version":1045,"timestamp":1623423430,"state":{"LEDstate":0},"metadata":{"LEDstate":{"timestamp":1623423430}}}.
72 45013 [iot_thread] [INFO] version: 1045
73 45013 [iot_thread] [INFO] version:1045, ulCurrentVersion:1043 

74 45016 [iot_thread] [INFO] Turn off LED Red
75 45016 [iot_thread] [INFO] the published payload:{"state":{"reported":{"LEDstate":0}},"clientToken": "token-45016"} 
 
76 45016 [iot_thread] [INFO] PUBLISH sent for topic $aws/things/aws_demo/shadow/update to broker with packet ID 9.


77 45295 [iot_thread] [INFO] Packet received. ReceivedBytes=2.
78 45295 [iot_thread] [INFO] Ack packet deserialized with result: MQTTSuccess.
79 45295 [iot_thread] [INFO] State record updated. New state=MQTTPublishDone.
80 45295 [iot_thread] [INFO] PUBACK received for packet id 9.


81 45295 [iot_thread] [INFO] Cleaned up outgoing publish packet with packet id 9.


82 47104 [iot_thread] [INFO] Successfully performed update.
.
.
.
83 50750 [iot_thread] [INFO] Packet received. ReceivedBytes=167.
84 50750 [iot_thread] [INFO] De-serialized incoming PUBLISH packet: DeserializerResult=MQTTSuccess.
85 50750 [iot_thread] [INFO] State record updated. New state=MQTTPubAckSend.
86 50750 [iot_thread] [INFO] pPublishInfo->pTopicName:$aws/things/aws_demo/shadow/update/delta.
87 50750 [iot_thread] [INFO] /update/delta json payload:{"version":1047,"timestamp":1623423435,"state":{"accelUpdate":1},"metadata":{"accelUpdate":{"timestamp":1623423435}}}.
88 50752 [iot_thread] [INFO] version: 1047
89 50752 [iot_thread] [INFO] version:1047, ulCurrentVersion:1045 

90 50954 [iot_thread] [INFO] Update accelerometer.
91 50956 [iot_thread] [INFO] the published payload:{"state":{"desired":{"accelUpdate":null},"reported":{"accel":{"x":-21,"y":-7,"z":996}}},"clientToken": "token-50956"} 
 
92 50959 [iot_thread] [INFO] PUBLISH sent for topic $aws/things/aws_demo/shadow/update to broker with packet ID 10.


93 51196 [iot_thread] [INFO] Packet received. ReceivedBytes=2.
94 51197 [iot_thread] [INFO] Ack packet deserialized with result: MQTTSuccess.
95 51197 [iot_thread] [INFO] State record updated. New state=MQTTPublishDone.
96 51197 [iot_thread] [INFO] PUBACK received for packet id 10.


97 51197 [iot_thread] [INFO] Cleaned up outgoing publish packet with packet id 10.


98 53006 [iot_thread] [INFO] Successfully performed update.
.
.
.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
