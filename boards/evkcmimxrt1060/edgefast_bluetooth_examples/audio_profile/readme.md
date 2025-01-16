# audio_profile

## Overview
This demo demonstrates the follow function:
1. There are five parts working in the demo: AWS cloud, Android app, audio demo (running on target board), U-disk and BlueTooth headset.
2. With an app running on the smart phone (Android phone), the end users could connect to AWS cloud and control the audio demo running on the target board EVK board through AWS cloud. Some operations like play, play next, pause, etc., could be used to control the media play functionalities. 
3. Audio demo running on the target board EVK board connects to the AWS through Wifi, also a connection could be established between the target board EVK board and a Bluetooth headset. To get the media resource (mp3 files) from the U-disk, a HS USB host is enabled, and a U-disk with mp3 files should be connected to target board EVK board via the USB port. After that, the audio demo will search the root directory of U-disk for the music files (now only mp3 files are supported), and upload the song file list to AWS, then the song list would be shown in the app running on the smart phone. Finally, the music could be played out via the Bluetooth headset once end user controls the app to play the mp3 file.

## Prepare the Demo
Before running the demo it is need to configure AWS IoT Console and update some of project files:  
1. Create AWS Account: https://console.aws.amazon.com/console/home
2. To create an AWS IOT policy based on the guide : https://docs.aws.amazon.com/freertos/latest/userguide/freertos-prereqs.html.
    1. Browse to the AWS IoT console(https://console.aws.amazon.com/iotv2/).
    2. In the navigation pane, choose "Secure", choose "Policies", and then choose "Create".
    3. Enter a name to identify your policy. For example, the policy name is "MusicPlayerPolicy".
    4. In the Add statements section, choose Advanced mode. Copy and paste the following JSON into the policy editor window.
    ```
    {
    "Version": "2012-10-17",
    "Statement": [
        {
        "Effect": "Allow",
        "Action": "iot:*",
        "Resource": "*"
        }
        ]
    }
    ```
    5. Choose "Create".
3. To create an IoT thing, private key, and certificate for your device based on the guide: https://docs.aws.amazon.com/freertos/latest/userguide/ble-demo.html. For more information about setting up, see the Getting Started with AWS IoT (https://docs.aws.amazon.com/iot/latest/developerguide/iot-gs.html).
    1. Open the "AWS IoT console"(https://console.aws.amazon.com/iot/), and from the navigation pane, choose "Manage", and then choose "Things".
    2. Choose "Create", and then choose "Create a single thing".
    3. Enter a name for your device, and then choose "Next". For example, the thing name is "MusicPlayer". Note, the max length of thing name cannot more than 23.
    4. Choose "Create certificate", choose "Activate", and then "download" the thing's certificate, public key, and private key.
    5. Choose "Attach a policy" to attach a policy to your certificate that grants your device access to AWS IoT operations.
    6. Choose the policy you just created and choose Register thing. Such as, choose the policy "MusicPlayerPolicy" created in step 2.d.
    7. Choose the thing that you just created from the list of registered things, and then choose "Interact" from your thing's page. Make a note of the AWS IoT REST API endpoint.
4. To configure your AWS IoT endpoint based on the guide: https://docs.aws.amazon.com/freertos/latest/userguide/freertos-prereqs.html.  
You need to update FreeRTOS with your AWS IoT endpoint so the application running on your board can send requests to the correct endpoint.
    1. Browse to the AWS IoT console(https://console.aws.amazon.com/iotv2/).
    2. In the navigation pane, choose "Settings".
    Your AWS IoT endpoint is displayed in "Endpoint". It should look like 1234567890123-ats.iot.us-west-2.amazonaws.com. Make a note of this endpoint.
    3. In the navigation pane, choose "Manage", and then choose "Things". Your device should have an AWS IoT thing name. Make a note of this name. Such as, the thing name "MusicPlayer" created in step 3.c.
    4. Open <MCUXpresso SDK>/boards/<board>/edgefast_bluetooth_examples/audio_profile/aws_clientcredential.h.
    5. Specify values for the following constants:
        - #define clientcredentialMQTT_BROKER_ENDPOINT "Your AWS IoT endpoint"  
        - #define clientcredentialIOT_THING_NAME "The AWS IoT thing name of your board"  
        - change clientcredentialWIFI_SSID as the connected WIFI SSID.  
        - change clientcredentialWIFI_PASSWORD as the connected WIFI password.  
        - suggest to change clientcredentialMQTT_BROKER_PORT as 443.  
5. To format your AWS IoT credentials based on the guide: https://docs.aws.amazon.com/freertos/latest/userguide/freertos-prereqs.html  
FreeRTOS needs the AWS IoT certificate and private keys associated with your registered thing and its permissions policies to successfully communicate with AWS IoT on behalf of your device.  
FreeRTOS is a C language project, and the certificate and private key must be specially formatted to be added to the project.
    1. Use the PEM-to-C-string.py (<MCUXpresso SDK>\middleware\aws_iot\amazon-freertos\tools\certificate_configuration) to generate the "aws_clientcredential_keys.h".
    2. Copy the file to <MCUXpresso SDK>/boards/<board>/edgefast_bluetooth_examples/audio_profile. This overwrites the existing file in the directory.  
Note, The certificate and private key are hard-coded for demonstration purposes only. Production-level applications should store these files in a secure location.
6. Open example's project and build it.
7. Connect a USB cable between the PC host and the OpenSDA USB port on the target board.
8. Connect U-disk to USB port on the target board through the USB OTG cable.
9. Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
10. Download the program to the target board.
11. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

## Prepare the Android application
The Android application requires Cognito service to authorize to AWS IoT in order to access device shadows. Use Amazon Cognito to create a new user pool and identity pool based on the guide: https://docs.aws.amazon.com/freertos/latest/userguide/ble-demo.html
1. To create an Amazon Cognito identity pool
    1. Open the Amazon Cognito console, and choose "Manage Identity Pools".
    2. Enter a name for your identity pool. Such as the pool name is "MusicPlayerIdentity", enable unauthenticated access and click "Create Pool".
    3. Choose "Create Pool".
    4. Edit the Unauth_Role policy and click "Allow", fill the follow content to the policy.
    ```
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
                "*"
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
    ```
    5. Copy the Identity pool ID in the next page.

2. After the above steps. The pool is created successfully.  
   Prepare one configuration file for the android app.  
   Prepare "AwsMusicControlPreferences.properties" file with yours AWS credentials. It's structure looks like this:
   ```
    customer_specific_endpoint=<REST API ENDPOINT>
    cognito_pool_id=<COGNITO POOL ID>
    thing_name=<THING NAME>
    region=<REGION>
   ```
    - customer_specific_endpoint is the endpoint that is configured in aws_clientcredential.h  
    - cognito_pool_id is the copied pool id in above step.  
    - thing_name is the created Thing name.  
    - region is the front part of the endpoint. 
    For example:
    ```
    customer_specific_endpoint=xxxxxx-ats.iot.us-west-2.amazonaws.com
    cognito_pool_id=us-west-2:5xxxx7-3xxxx9-4xxx5-axxf-0xxxxxxxxxxb
    thing_name=MusicPlayer
    region=us-west-2
    ```
3. To run Android application, do either:
    1. Install and run pre-build apk on Android device (<MCUXpresso SDK>\boards\<board name>\edgefast_bluetooth_examples\audio_pofile\android_app\AwsMusicControl.apk)
    2. Open project in Android Studio, build it, attach Android device and Run application
    Application requires at least Android version 5.1 (Android SDK 22).  
    Then in both cases when asked select AwsMusicControlPreferences.properties file with AWS IoT preferences. Then control the music.

## Running the demo
1. The log below shows the output of the demo in the terminal window. The log can be different based on your WiFi network configuration and based on the actions, which you have done in the Android application.
2. After the log "Bluetooth initialized", the shell command can be used to connect to BT headset.
    1. The commands are as follow:
    ```
    "bt": BT related function
        USAGE: bt [connectaddress|finddevice|connectdevice|disconnect|deletedevice]
        connectaddress connect to the device of the address parameter,for example: bt connectaddress xx:xx:xx:xx:xx:xx. Address format(LSB-MSB): xx:xx:xx:xx:xx:xx
        finddevice     start to find BT devices
        connectdevice  connect to the device that is found, for example: bt connectdevice n (from 1)
        disconnect     disconnect current connection
        deletedevice   delete all devices
    ```
    2. Use "connectaddress".  
    Input "bt connectaddress xx:xx:xx:xx:xx:xx Address format(LSB-MSB)", wait the "A2DP_CONNECT_CNF" log.
    3. Use "finddevice" and "connectdevice".  
    Input "bt finddevice", wait the found device name list to be print as follow. These devices names are numbered, then input "bt connectdevice n", the n is the connected device number.
    ```
    The [1] device:heaset1
    Received HCI_COMMAND_STATUS_EVENT.
        Command Status: 0x00
        Num Command Packets: 1 (0x01)
        Command Opcode: 0x0419 (HCI_Command)
    Received HCI_REMOTE_NAME_REQUEST_COMPLETE_EVENT.
    The [2] device:heaset2
    Received HCI_COMMAND_STATUS_EVENT.
        Command Status: 0x00
        Num Command Packets: 1 (0x01)
        Command Opcode: 0x0419 (HCI_Command)
    Received HCI_REMOTE_NAME_REQUEST_COMPLETE_EVENT.
    The [3] device:heaset3
    Received HCI_COMMAND_STATUS_EVENT.
        Command Status: 0x00
        Num Command Packets: 1 (0x01)
        Command Opcode: 0x0419 (HCI_Command)
    Received HCI_REMOTE_NAME_REQUEST_COMPLETE_EVENT.
    ```
    4. Use "disconnect"  
    Input "bt disconnect", then the connection with the headset is released.
    5. Use "deletedevice"  
    Input "bt deletedevice", then all the connected devices' bound and authentication information is deleted.
3. After the headset is connected successfully, Android application can be used to control the music playing. The music control buttons and the music list are displayed in the app.

```
usb host init done
0 21 [main_task] Warning: could not clean-up old crypto objects. 6 

1 21 [main_task] Initializing Wi-Fi...

MAC Address: 9C:50:D1:44:F5:7F 
2 3166 [main_task] Wi-Fi initialized successfully.

3 3168 [main_task] Connecting to: Cher's iPhone

4 12932 [main_task] Wi-Fi connected

5 12933 [main_task] IP Address acquired: 172.20.10.3

6 12934 [MQTT] [INFO] Creating a TLS connection to a2nxzv2h17k05v.ats.iot.cn-north-1.amazonaws.com.cn:8883.
7 15775 [MQTT] [INFO] (Network connection 202539dc) TLS handshake successful.
8 15775 [MQTT] [INFO] (Network connection 202539dc) Connection to a2nxzv2h17k05v.ats.iot.cn-north-1.amazonaws.com.cn established.
9 15775 [MQTT] [INFO] Creating an MQTT connection to the broker.
10 16048 [MQTT] [INFO] MQTT connection established with the broker.
11 16048 [MQTT] [INFO] Successfully connected to MQTT broker.
12 16049 [SHADOW_DEV] [INFO] MQTT Agent is connected. Initializing shadow device task.
13 16049 [SHADOW_DEV] [INFO] Sending subscribe request to agent for shadow topics.
14 16056 [SHADOW_APP] [INFO] MQTT Agent is connected. Initializing shadow update task.
15 16057 [SHADOW_APP] [INFO] Sending subscribe request to agent for shadow topics.
16 16310 [SHADOW_APP] [INFO] Received subscribe ack for shadow update topics.
17 16311 [SHADOW_DEV] [INFO] Successfully subscribed to shadow update topics.
18 16311 [SHADOW_DEV] [INFO] Publishing to /get message using client token 16311.
19 16311 [MQTT] [INFO] Publishing message to $aws/things/aws_audio_profile/shadow/get.

20 16314 [SHADOW_APP] [INFO] Publishing to /update with following client token 16314.
21 16314 [MQTT] [INFO] Publishing message to $aws/things/aws_audio_profile/shadow/update.

22 16317 [SHADOW_DEV] [INFO] Successfully sent a publish message to /get topic.
23 16467 [MQTT] [INFO] Ack packet deserialized with result: MQTTSuccess.
24 16468 [MQTT] [INFO] State record updated. New state=MQTTPublishDone.
25 16468 [MQTT] [INFO] Ack packet deserialized with result: MQTTSuccess.
26 16469 [MQTT] [INFO] State record updated. New state=MQTTPublishDone.
27 16520 [MQTT] [INFO] De-serialized incoming PUBLISH packet: DeserializerResult=MQTTSuccess.
28 16520 [MQTT] [INFO] State record updated. New state=MQTTPubAckSend.
29 16520 [MQTT] [WARN] Received rejected response for get with error code 404.
30 16522 [SHADOW_DEV] [WARN] Cannot find a shadow document in cloud.
31 16574 [MQTT] [INFO] De-serialized incoming PUBLISH packet: DeserializerResult=MQTTSuccess.
32 16574 [MQTT] [INFO] State record updated. New state=MQTTPubAckSend.
33 16575 [MQTT] [INFO] Received accepted response for update with token 16314. 
Bluetooth initialized

Copyright  2022  NXP

>> 34 17522 [SHADOW_DEV] [INFO] Publishing to /get message using client token 17522.
35 17522 [MQTT] [INFO] Publishing message to $aws/things/aws_audio_profile/shadow/get.

36 17524 [SHADOW_DEV] [INFO] Successfully sent a publish message to /get topic.
37 17724 [MQTT] [INFO] Ack packet deserialized with result: MQTTSuccess.
38 17724 [MQTT] [INFO] State record updated. New state=MQTTPublishDone.
39 17777 [MQTT] [INFO] De-serialized incoming PUBLISH packet: DeserializerResult=MQTTSuccess.
40 17777 [MQTT] [INFO] State record updated. New state=MQTTPubAckSend.
41 17779 [SHADOW_DEV] [INFO] Received an accepted response for shadow GET request.
```

## Note:
1. This demo could NOT function with the default setting provided in SDK package because a AWS account is mandatory to run to the demo, the end users must create their own AWS account and configure the IoT Thing before the functionality of the demo could be used.  
Also some information specified by the end customers, like Thing name, Wifi SSID Wifi password, etc., must be updated accordingly before the demo would work. Check (Prepare the Demo) to get the detailed guidance of the configuration steps.
2. The music files names in U-disk need be english.
3. The volume of audio adjustment is not supported.

## Supported Boards
- [MIMXRT1170-EVKB](../../_boards/evkbmimxrt1170/edgefast_bluetooth_examples/audio_profile/example_board_readme.md)
- [MIMXRT1060-EVKC](../../_boards/evkcmimxrt1060/edgefast_bluetooth_examples/audio_profile/example_board_readme.md)
