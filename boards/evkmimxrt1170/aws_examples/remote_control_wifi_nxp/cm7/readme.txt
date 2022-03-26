Overview
========
This example demonstrates how the board can be controlled by Android mobile application with usage the AWS IoT.
User LEDs on the board can be controlled and also an action can be invoked to read data from accelerometer and report it to AWS IoT device shadow.
Please take look into section "Board settings" if there are some board limitations to control LEDs or read data from accelerometer.

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
The communication with accelerometer sensor is disabled by default, because some boards have not accelerometer sensor.
If the board has accelerometer sensor, the using of it can be enabled by definition of project macro: BOARD_ACCEL_FXOS.

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

5.  Connect a USB cable between the PC host and the OpenSDA USB port on the target board.

6.  Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control

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
The log below shows the output of the demo in the terminal window. The log can be different based on your Wi-Fi network configuration and based on the actions, which you have done in the Android application.

Android application displays Accelerometer data for all three axis and status of LEDs which is split into Desired and Reported section. Desired value is value wanted by user and Reported value is actual value of LEDs on device.

- When you turn on/off some of RGB LED in the Android application, the change should be visible on RGB LED on the board.
- When you refresh accelerometer data on Android application, you should see changes in the accelerometer data.

Every mentioned action takes approximately 1-3 seconds.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
0 114 [Tmr Svc] Write certificate...

1 130 [iot_thread] [INFO ][DEMO][130] ---------STARTING DEMO---------


2 132 [iot_thread] [INFO ][INIT][132] SDK successfully initialized.

MAC Address:  0:13:43:91:8C:9D 
[net] Initialized TCP/IP networking stack

3 3443 [iot_thread] Connecting to nxp .....

4 12155 [wlcmgr] Connected to with IP = [192.168.1.107]

5 12193 [iot_thread] [INFO ][DEMO][12193] Successfully initialized the demo. Network type for the demo: 1

6 12193 [iot_thread] [INFO] Create a TCP connection to a2zcot8a2tqh6c-ats.iot.us-east-2.amazonaws.com:8883.
7 13297 [iot_thread] [INFO] Packet received. ReceivedBytes=2.
8 13299 [iot_thread] [INFO] CONNACK session present bit not set.
9 13299 [iot_thread] [INFO] Connection accepted.
10 13299 [iot_thread] [INFO] Received MQTT CONNACK successfully from broker.
11 13299 [iot_thread] [INFO] MQTT connection established with the broker.
12 13299 [iot_thread] [INFO] MQTT connection successfully established with broker.


13 13299 [iot_thread] [INFO] A clean MQTT connection is established. Cleaning up all the stored outgoing publishes.


14 13300 [iot_thread] [INFO] SUBSCRIBE topic $aws/things/aws_demo/shadow/delete/accepted to broker.


15 13445 [iot_thread] [INFO] Packet received. ReceivedBytes=3.
16 13445 [iot_thread] [INFO] MQTT_PACKET_TYPE_SUBACK.


17 15455 [iot_thread] [INFO] SUBSCRIBE topic $aws/things/aws_demo/shadow/delete/rejected to broker.


18 15630 [iot_thread] [INFO] Packet received. ReceivedBytes=3.
19 15630 [iot_thread] [INFO] MQTT_PACKET_TYPE_SUBACK.


20 17640 [iot_thread] [INFO] the published payload: 
 
21 17641 [iot_thread] [INFO] PUBLISH sent for topic $aws/things/aws_demo/shadow/delete to broker with packet ID 3.


22 17857 [iot_thread] [INFO] Packet received. ReceivedBytes=2.
23 17857 [iot_thread] [INFO] Ack packet deserialized with result: MQTTSuccess.
24 17857 [iot_thread] [INFO] State record updated. New state=MQTTPublishDone.
25 17857 [iot_thread] [INFO] PUBACK received for packet id 3.


26 17857 [iot_thread] [INFO] Cleaned up outgoing publish packet with packet id 3.


27 17895 [iot_thread] [INFO] Packet received. ReceivedBytes=92.
28 17895 [iot_thread] [INFO] De-serialized incoming PUBLISH packet: DeserializerResult=MQTTSuccess.
29 17895 [iot_thread] [INFO] State record updated. New state=MQTTPubAckSend.
30 17895 [iot_thread] [INFO] pPublishInfo->pTopicName:$aws/things/aws_demo/shadow/delete/accepted.
31 17895 [iot_thread] [INFO] Received an MQTT incoming publish on /delete/accepted topic.
32 19705 [iot_thread] [INFO] UNSUBSCRIBE sent topic $aws/things/aws_demo/shadow/delete/accepted to broker.


33 19850 [iot_thread] [INFO] Packet received. ReceivedBytes=2.
34 19850 [iot_thread] [INFO] MQTT_PACKET_TYPE_UNSUBACK.


35 21860 [iot_thread] [INFO] UNSUBSCRIBE sent topic $aws/things/aws_demo/shadow/delete/rejected to broker.


36 22035 [iot_thread] [INFO] Packet received. ReceivedBytes=2.
37 22035 [iot_thread] [INFO] MQTT_PACKET_TYPE_UNSUBACK.


38 24045 [iot_thread] [INFO] SUBSCRIBE topic $aws/things/aws_demo/shadow/update/delta to broker.


39 24195 [iot_thread] [INFO] Packet received. ReceivedBytes=3.
40 24195 [iot_thread] [INFO] MQTT_PACKET_TYPE_SUBACK.


41 26205 [iot_thread] [INFO] the published payload:{"state":{"desired":{"LEDstate":0},"reported":{"LEDstate":0,"accel":{"x":0,"y":0,"z":0},"LEDinfo":{"isRgbLed":false,"colors":["green"]}}},"clientToken": "token-26205"} 
 
42 26206 [iot_thread] [INFO] PUBLISH sent for topic $aws/things/aws_demo/shadow/update to broker with packet ID 7.


43 26509 [iot_thread] [INFO] Packet received. ReceivedBytes=2.
44 26509 [iot_thread] [INFO] Ack packet deserialized with result: MQTTSuccess.
45 26509 [iot_thread] [INFO] State record updated. New state=MQTTPublishDone.
46 26509 [iot_thread] [INFO] PUBACK received for packet id 7.


47 26509 [iot_thread] [INFO] Cleaned up outgoing publish packet with packet id 7.


48 28318 [iot_thread] [INFO] AWS Remote Control Demo initialized.
49 28318 [iot_thread] [INFO] Use mobile application to control the remote device.
.
.
.
50 45680 [iot_thread] [INFO] Packet received. ReceivedBytes=161.
51 45680 [iot_thread] [INFO] De-serialized incoming PUBLISH packet: DeserializerResult=MQTTSuccess.
52 45680 [iot_thread] [INFO] State record updated. New state=MQTTPubAckSend.
53 45680 [iot_thread] [INFO] pPublishInfo->pTopicName:$aws/things/aws_demo/shadow/update/delta.
54 45680 [iot_thread] [INFO] /update/delta json payload:{"version":1051,"timestamp":1623426921,"state":{"LEDstate":1},"metadata":{"LEDstate":{"timestamp":1623426921}}}.
55 45680 [iot_thread] [INFO] version: 1051
56 45680 [iot_thread] [INFO] version:1051, ulCurrentVersion:0 

57 45884 [iot_thread] [INFO] Turn on LED
58 45884 [iot_thread] [INFO] the published payload:{"state":{"reported":{"LEDstate":1}},"clientToken": "token-45884"} 
 
59 45885 [iot_thread] [INFO] PUBLISH sent for topic $aws/things/aws_demo/shadow/update to broker with packet ID 8.


60 46144 [iot_thread] [INFO] Packet received. ReceivedBytes=2.
61 46144 [iot_thread] [INFO] Ack packet deserialized with result: MQTTSuccess.
62 46144 [iot_thread] [INFO] State record updated. New state=MQTTPublishDone.
63 46144 [iot_thread] [INFO] PUBACK received for packet id 8.


64 46144 [iot_thread] [INFO] Cleaned up outgoing publish packet with packet id 8.


65 47953 [iot_thread] [INFO] Successfully performed update.
.
.
.
66 49423 [iot_thread] [INFO] Packet received. ReceivedBytes=161.
67 49423 [iot_thread] [INFO] De-serialized incoming PUBLISH packet: DeserializerResult=MQTTSuccess.
68 49423 [iot_thread] [INFO] State record updated. New state=MQTTPubAckSend.
69 49423 [iot_thread] [INFO] pPublishInfo->pTopicName:$aws/things/aws_demo/shadow/update/delta.
70 49423 [iot_thread] [INFO] /update/delta json payload:{"version":1053,"timestamp":1623426925,"state":{"LEDstate":0},"metadata":{"LEDstate":{"timestamp":1623426925}}}.
71 49423 [iot_thread] [INFO] version: 1053
72 49423 [iot_thread] [INFO] version:1053, ulCurrentVersion:1051 

73 49625 [iot_thread] [INFO] Turn off LED
74 49625 [iot_thread] [INFO] the published payload:{"state":{"reported":{"LEDstate":0}},"clientToken": "token-49625"} 
 
75 49626 [iot_thread] [INFO] PUBLISH sent for topic $aws/things/aws_demo/shadow/update to broker with packet ID 9.


76 49875 [iot_thread] [INFO] Packet received. ReceivedBytes=2.
77 49875 [iot_thread] [INFO] Ack packet deserialized with result: MQTTSuccess.
78 49875 [iot_thread] [INFO] State record updated. New state=MQTTPublishDone.
79 49875 [iot_thread] [INFO] PUBACK received for packet id 9.


80 49875 [iot_thread] [INFO] Cleaned up outgoing publish packet with packet id 9.


81 51684 [iot_thread] [INFO] Successfully performed update.
.
.
.
82 54021 [iot_thread] [INFO] Packet received. ReceivedBytes=167.
83 54021 [iot_thread] [INFO] De-serialized incoming PUBLISH packet: DeserializerResult=MQTTSuccess.
84 54021 [iot_thread] [INFO] State record updated. New state=MQTTPubAckSend.
85 54021 [iot_thread] [INFO] pPublishInfo->pTopicName:$aws/things/aws_demo/shadow/update/delta.
86 54021 [iot_thread] [INFO] /update/delta json payload:{"version":1055,"timestamp":1623426930,"state":{"accelUpdate":1},"metadata":{"accelUpdate":{"timestamp":1623426930}}}.
87 54021 [iot_thread] [INFO] version: 1055
88 54021 [iot_thread] [INFO] version:1055, ulCurrentVersion:1053 

89 54022 [iot_thread] [INFO] Update accelerometer.
90 54024 [iot_thread] [INFO] the published payload:{"state":{"desired":{"accelUpdate":null},"reported":{"accel":{"x":-5,"y":-15,"z":1001}}},"clientToken": "token-54024"} 
 
91 54024 [iot_thread] [INFO] PUBLISH sent for topic $aws/things/aws_demo/shadow/update to broker with packet ID 10.


92 54301 [iot_thread] [INFO] Packet received. ReceivedBytes=2.
93 54301 [iot_thread] [INFO] Ack packet deserialized with result: MQTTSuccess.
94 54301 [iot_thread] [INFO] State record updated. New state=MQTTPublishDone.
95 54301 [iot_thread] [INFO] PUBACK received for packet id 10.


96 54301 [iot_thread] [INFO] Cleaned up outgoing publish packet with packet id 10.


97 56110 [iot_thread] [INFO] Successfully performed update.
.
.
.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
