Overview
========
This demo demonstrates the follow function:
1. There are five parts working in the demo: AWS cloud, Android app, audio demo (running on RT1060), U-disk and BlueTooth headset.
2. With an app running on the smart phone (Android phone), the end users could connect to AWS cloud and control the audio demo running on the RT1060 EVK board through AWS cloud. Some operations like play, play next, pause, etc., could be used to control the media play functionalities. 
3. Audio demo running on the RT1060 EVK board connects to the AWS through Wifi, also a connection could be established between the RT1060 EVK board and a Bluetooth headset. To get the media resource (mp3 files) from the U-disk, a HS USB host is enabled, and a U-disk with mp3 files should be connected to RT1060 EVK board via the USB port. After that, the audio demo will search the root directory of U-disk for the music files (now only mp3 files are supported), and upload the song file list to AWS, then the song list would be shown in the app running on the smart phone. Finally, the music could be played out via the Bluetooth headset once end user controls the app to play the mp3 file.
Note:
1. This demo could NOT function with the default setting provided in SDK package because a AWS account is mandatory to run to the demo, the end users must create their owner AWS account and configure the IoT Thing before the functionality of the demo could be used. Also some information specified by the end customers, like Thing name, Wifi SSID Wifi password, etc., must be updated accordingly before the demo would work. Check ¡°Prepare the Demo¡° to get the detailed guidance of the configuration steps.
2. The music files names in U-disk need be english.
3. The volume of audio adjustment is not supported.


Toolchain supported
===================
- MCUXpresso  11.3.0
- IAR embedded Workbench  8.50.9
- GCC ARM Embedded  9.3.1

Hardware requirements
=====================
- Micro USB cable
- evkmimxrt1060 board
- Personal Computer
- One of the following WiFi modules:
  - AzureWave AW-AM457-uSD
- U-disk and usb otg cable
- BlueTooth headset
- Android cellphone

Board settings
==============

Jumper settings for AzureWave AW-AM457-uSD Module:
  - J11 2-3: VIO_SD 3.3V (Voltage level of SDIO pins is 3.3V)
  - J2  1-2: 3.3V VIO_uSD (Power Supply from uSD connector)
  - J4  2-3: 3.3V VIO

The hardware should be reworked according to the hardware rework guide for evkmimxrt1060 and AW-AM457-uSD.
The pin connect for UART HCI as the following table,
------------------------------------------------------------------------------------
PIN NAME | AW-AM457-USD |   I.MXRT1060   | PIN NAME OF RT1060 | GPIO NAME OF RT1060
------------------------------------------------------------------------------------
UART_TXD |  J10(pin 4)  |   J22(pin 1)   |    LPUART3_RXD     | GPIO_AD_B1_07
UART_RXD |  J10(pin 2)  |   J22(pin 2)   |    LPUART3_TXD     | GPIO_AD_B1_06
UART_RTS |  J10(pin 6)  |   J23(pin 3)   |    LPUART3_CTS     | GPIO_AD_B1_04
UART_CTS |  J10(pin 8)  |   J23(pin 4)   |    LPUART3_RTS     | GPIO_AD_B1_05
GND      |  J6(pin 7)   |   J25(pin 7)   |    GND             | GND
------------------------------------------------------------------------------------
Note:
After downloaded binary into qspiflash and boot from qspiflash directly, 
please reset the board by pressing SW9 or power off and on the board to run the application.


Prepare the Demo
================
Before running the demo it is need to configure AWS IoT Console and update some of project files:

1. Create AWS Account: https://console.aws.amazon.com/console/home

2. To create an AWS IOT policy based on the guide : https://docs.aws.amazon.com/freertos/latest/userguide/freertos-prereqs.html.

a. Browse to the AWS IoT console(https://console.aws.amazon.com/iotv2/).
c. In the navigation pane, choose "Secure", choose "Policies", and then choose "Create".
d. Enter a name to identify your policy. For example, the policy name is "MusicPlayerPolicy".
e. In the Add statements section, choose Advanced mode. Copy and paste the following JSON into the policy editor window.
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
f. Choose "Create".

3. To create an IoT thing, private key, and certificate for your device based on the guide: https://docs.aws.amazon.com/freertos/latest/userguide/ble-demo.html. For more information about setting up, see the Getting Started with AWS IoT (https://docs.aws.amazon.com/iot/latest/developerguide/iot-gs.html).

a. Open the "AWS IoT console"(https://console.aws.amazon.com/iot/), and from the navigation pane, choose "Manage", and then choose "Things".
b. Choose "Create", and then choose "Create a single thing".
c. Enter a name for your device, and then choose "Next". For example, the thing name is "MusicPlayer". Note, the max length of thing name cannot more than 23.
d. Choose "Create certificate", choose "Activate", and then "download" the thing's certificate, public key, and private key.
e. Choose "Attach a policy" to attach a policy to your certificate that grants your device access to AWS IoT operations.
f. Choose the policy you just created and choose Register thing. Such as, choose the policy "MusicPlayerPolicy" created in step 2.d.
g. Choose the thing that you just created from the list of registered things, and then choose "Interact" from your thing's page. Make a note of the AWS IoT REST API endpoint.

4. To configure your AWS IoT endpoint based on the guide: https://docs.aws.amazon.com/freertos/latest/userguide/freertos-prereqs.html
You need to update FreeRTOS with your AWS IoT endpoint so the application running on your board can send requests to the correct endpoint.
a. Browse to the AWS IoT console(https://console.aws.amazon.com/iotv2/).
b. In the navigation pane, choose "Settings".
   Your AWS IoT endpoint is displayed in "Endpoint". It should look like 1234567890123-ats.iot.us-west-2.amazonaws.com. Make a note of this endpoint.
c. In the navigation pane, choose "Manage", and then choose "Things". Your device should have an AWS IoT thing name. Make a note of this name. Such as, the thing name "MusicPlayer" created in  step 3.c.
d. Open <MCUXpresso SDK>/rtos/freertos/demos/include/aws_clientcredential.h.
e. Specify values for the following constants:
   o #define clientcredentialMQTT_BROKER_ENDPOINT "Your AWS IoT endpoint"
   o #define clientcredentialIOT_THING_NAME "The AWS IoT thing name of your board"
   o change clientcredentialWIFI_SSID as the connected WIFI SSID.
   o change clientcredentialWIFI_PASSWORD as the connected WIFI password.
   o suggest to change clientcredentialMQTT_BROKER_PORT as 443.

5. To format your AWS IoT credentials based on the guide: https://docs.aws.amazon.com/freertos/latest/userguide/freertos-prereqs.html
FreeRTOS needs the AWS IoT certificate and private keys associated with your registered thing and its permissions policies to successfully communicate with AWS IoT on behalf of your device.
FreeRTOS is a C language project, and the certificate and private key must be specially formatted to be added to the project.
a. In a browser window, open <MCUXpresso SDK>/rtos/freertos/tools/certificate_configuration/CertificateConfigurator.html.
b. Under "Certificate PEM file", choose the ID-certificate.pem.crt that you downloaded from the AWS IoT console.
c. Under "Private Key PEM file", choose the ID-private.pem.key that you downloaded from the AWS IoT console.
d. Choose "Generate and save aws_clientcredential_keys.h", and then save the file in <MCUXpresso SDK>/rtos/freertos/demos/include. This overwrites the existing file in the directory.
Note, The certificate and private key are hard-coded for demonstration purposes only. Production-level applications should store these files in a secure location.

6. Open example's project and build it.

7. Connect a USB cable between the PC host and the OpenSDA USB port on the target board.

8. Connect U-disk to J9 on the target board through the USB OTG cable.

9. Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control

10. Download the program to the target board.

11. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.


Prepare the Android application
The Android application requires Cognito service to authorize to AWS IoT in order to access device shadows. Use Amazon Cognito to create a new user pool and identity pool based on the guide: https://docs.aws.amazon.com/freertos/latest/userguide/ble-demo.html

1. To create an Amazon Cognito identity pool

a. Open the Amazon Cognito console, and choose "Manage Identity Pools".
b. Enter a name for your identity pool. Such as the pool name is "MusicPlayerIdentity", enable unauthenticated access and click "Create Pool".
c. Choose "Create Pool".
d. Edit the Unauth_Role policy and click "Allow", fill the follow content to the policy.
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
e. Copy the Identity pool ID in the next page.

2. After the above steps. The pool is created successfully.
   Prepare one configuration file for the android app.
   Prepare "AwsMusicControlPreferences.properties" file with yours AWS credentials. It's structure looks like this:
    customer_specific_endpoint=<REST API ENDPOINT>
    cognito_pool_id=<COGNITO POOL ID>
    thing_name=<THING NAME>
    region=<REGION>
   o customer_specific_endpoint is the endpoint that is configured in aws_clientcredential.h
   o cognito_pool_id is the copied pool id in above step.
   o thing_name is the created Thing name.
   o region is the front part of the endpoint.
  for example:
  customer_specific_endpoint=xxxxxx-ats.iot.us-west-2.amazonaws.com
  cognito_pool_id=us-west-2:5xxxx7-3xxxx9-4xxx5-axxf-0xxxxxxxxxxb
  thing_name=MusicPlayer
  region=us-west-2

3. To run Android application, do either:
   a) Install and run pre-build apk on Android device (<MCUXpresso SDK>\boards\evkmimxrt1060\ethermind_examples\ethermind_audio_android\AwsMusicControl.apk)
   b) Open project in Android Studio, build it, attach Android device and Run application

   Application requires at least Android version 5.1 (Android SDK 22).

   Then in both cases when asked select AwsMusicControlPreferences.properties file with AWS IoT preferences. Then control the music.


Running the demo
================
1. The log below shows the output of the demo in the terminal window. The log can be different based on your WiFi network configuration and based on the actions, which you have done in the Android application.

2. After the log "Use mobile application to control the remote device.", the shell command can be used to connect to BT headset.

a. The commands are as follow:
    "bt": BT related function
      USAGE: bt [connect |finddevice|connectdevice]
        connectaddress connect to the device of the address parameter,for example: bt connectaddress xx:xx:xx:xx:xx:xx. Address format(LSB-MSB): xx:xx:xx:xx:xx:xx
        finddevice     start to find BT devices
        connectdevice  connect to the device that is found, for example: bt connectdevice n (from 1)
        disconnect     disconnect current connection
        deletedevice   delete all devices
b. Use "connectaddress".
   Input "bt connectaddress xx:xx:xx:xx:xx:xx Address format(LSB-MSB)", wait the "A2DP_CONNECT_CNF" log.
c. Use "finddevice" and "connectdevice".
   Input "bt finddevice", wait the found device name list to be print as follow. These devices names are numbered, then input "bt connectdevice n", the n is the connected device number.
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
d. Use "disconnect"
   Input "bt disconnect", then the connection with the headset is released.
e. Use "deletedevice"
   Input "bt deletedevice", then all the connected devices' bound and authentication information is deleted.

3. After the headset is connected successfully, Android application can be used to control the music playing. The music control buttons and the music list are displayed in the app.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
usb host init done

**********************************
Maestro audio solutions demo start
**********************************


SHELL build: 0 1 [main_task] Starting WiFi...
Sep 28 2020
Copyright  2020  NXP
>> MAC Address: D8:C0:A6:C0:B0:F1 
[net] Initialized TCP/IP networking stack
[wm_wlan] WLAN_REASON_INITIALIZED

[wm_wlan] Connecting to NXPOPEN .....

1 3641 [main_task] WiFi module initialized.
[wm_wlan] Connected to with IP = [192.168.0.234]

2 15943 [main_task] WiFi connected to AP NXPOPEN.
3 15943 [main_task] IP Address acquired 192.168.0.234
4 15944 [AWS-RemoteCtrl] [INFO ][INIT][lu] SDK successfully initialized.
5 15944 [AWS-RemoteCtrl] [INFO ][Shadow][lu] Shadow library successfully initialized.

###################################################################
###
### EtherMind Bluetooth Evaluation Application
###
### Profiles    : A2DP/SPP/PAN (Selected from EtherMind.conf)
###################################################################

Reading the configuration ...
Failed to read BT_UART_PORT from the Config
Failed to read BT_UART_BAUDRATE from the Config
Failed to read BD_ADDRESS from the Config

Initializing EtherMind ...

Performing Bluetooth ON ...
Initializing A2DP ... 
OK

Bluetooth ON Initialization Completed.
Local Device Addr: D8:C0:A6:C0:B0:F0

Starting A2DP ...
A2DP Register Codec ... OK
6 29862 [AWS-RemoteCtrl] [INFO ][MQTT][lu] Establishing new MQTT connection.
7 29868 [AWS-RemoteCtrl] [INFO ][MQTT][lu] Anonymous metrics (SDK language, SDK version) will be provided to AWS IoT. Recompile with AWS_IOT_MQTT_ENABLE_METRICS set to 0 to disable.
8 29871 [AWS-RemoteCtrl] [INFO ][MQTT][lu] (MQTT connection 0x2023e868, CONNECT operation 0x2023f7e0) Waiting for operation completion.
9 30209 [AWS-RemoteCtrl] [INFO ][MQTT][lu] (MQTT connection 0x2023e868, CONNECT operation 0x2023f7e0) Wait complete with result SUCCESS.
10 30209 [AWS-RemoteCtrl] [INFO ][MQTT][lu] New MQTT connection 0x20237c90 established.
11 30210 [AWS-RemoteCtrl] [INFO ][MQTT][lu] (MQTT connection 0x2023e868) SUBSCRIBE operation scheduled.
12 30210 [AWS-RemoteCtrl] [INFO ][MQTT][lu] (MQTT connection 0x2023e868, SUBSCRIBE operation 0x2023fc48) Waiting for operation completion.
13 30491 [AWS-RemoteCtrl] [INFO ][MQTT][lu] (MQTT connection 0x2023e868, SUBSCRIBE operation 0x2023fc48) Wait complete with result SUCCESS.
14 30492 [AWS-RemoteCtrl] [INFO ][MQTT][lu] (MQTT connection 0x2023e868) SUBSCRIBE operation scheduled.
15 30492 [AWS-RemoteCtrl] [INFO ][MQTT][lu] (MQTT connection 0x2023e868, SUBSCRIBE operation 0x2023f7e0) Waiting for operation completion.
16 30806 [AWS-RemoteCtrl] [INFO ][MQTT][lu] (MQTT connection 0x2023e868, SUBSCRIBE operation 0x2023f7e0) Wait complete with result SUCCESS.
17 30809 [AWS-RemoteCtrl] [INFO ][MQTT][lu] (MQTT connection 0x2023e868) MQTT PUBLISH operation queued.
18 31151 [iot_thread] [INFO ][Shadow][lu] Shadow DELETE of MusicPlayer was ACCEPTED.
19 31152 [AWS-RemoteCtrl] [INFO ][MQTT][lu] (MQTT connection 0x2023e868) UNSUBSCRIBE operation scheduled.
20 31152 [AWS-RemoteCtrl] [INFO ][MQTT][lu] (MQTT connection 0x2023e868, UNSUBSCRIBE operation 0x2023f7e0) Waiting for operation completion.
21 31677 [AWS-RemoteCtrl] [INFO ][MQTT][lu] (MQTT connection 0x2023e868, UNSUBSCRIBE operation 0x2023f7e0) Wait complete with result SUCCESS.
22 31677 [AWS-RemoteCtrl] [INFO ][MQTT][lu] (MQTT connection 0x2023e868) UNSUBSCRIBE operation scheduled.
23 31678 [AWS-RemoteCtrl] [INFO ][MQTT][lu] (MQTT connection 0x2023e868, UNSUBSCRIBE operation 0x2023f7e0) Waiting for operation completion.
24 31961 [AWS-RemoteCtrl] [INFO ][MQTT][lu] (MQTT connection 0x2023e868, UNSUBSCRIBE operation 0x2023f7e0) Wait complete with result SUCCESS.
25 31962 [AWS-RemoteCtrl] [INFO ][Shadow][lu] (MusicPlayer) Modifying Shadow DELTA callback.
26 31962 [AWS-RemoteCtrl] [INFO ][Shadow][lu] (MusicPlayer) Adding new DELTA callback.
27 31963 [AWS-RemoteCtrl] [INFO ][MQTT][lu] (MQTT connection 0x2023e868) SUBSCRIBE operation scheduled.
28 31963 [AWS-RemoteCtrl] [INFO ][MQTT][lu] (MQTT connection 0x2023e868, SUBSCRIBE operation 0x2023fde8) Waiting for operation completion.
29 32263 [AWS-RemoteCtrl] [INFO ][MQTT][lu] (MQTT connection 0x2023e868, SUBSCRIBE operation 0x2023fde8) Wait complete with result SUCCESS.
30 32263 [AWS-RemoteCtrl] [INFO ][Shadow][lu] (MusicPlayer) Shadow DELTA callback operation complete with result SUCCESS.
31 32263 [AWS-RemoteCtrl] [INFO ][Shadow][lu] (MusicPlayer) Modifying Shadow UPDATED callback.
32 32264 [AWS-RemoteCtrl] [INFO ][Shadow][lu] (MusicPlayer) Shadow UPDATED callback operation complete with result SUCCESS.
33 32265 [AWS-RemoteCtrl] [INFO ][MQTT][lu] (MQTT connection 0x2023e868) SUBSCRIBE operation scheduled.
34 32265 [AWS-RemoteCtrl] [INFO ][MQTT][lu] (MQTT connection 0x2023e868, SUBSCRIBE operation 0x2023f898) Waiting for operation completion.
35 32557 [AWS-RemoteCtrl] [INFO ][MQTT][lu] (MQTT connection 0x2023e868, SUBSCRIBE operation 0x2023f898) Wait complete with result SUCCESS.
36 32558 [AWS-RemoteCtrl] [INFO ][MQTT][lu] (MQTT connection 0x2023e868) SUBSCRIBE operation scheduled.
37 32558 [AWS-RemoteCtrl] [INFO ][MQTT][lu] (MQTT connection 0x2023e868, SUBSCRIBE operation 0x2023f898) Waiting for operation completion.
38 32849 [AWS-RemoteCtrl] [INFO ][MQTT][lu] (MQTT connection 0x2023e868, SUBSCRIBE operation 0x2023f898) Wait complete with result SUCCESS.
39 32853 [AWS-RemoteCtrl] [INFO ][MQTT][lu] (MQTT connection 0x2023e868) MQTT PUBLISH operation queued.
40 33170 [iot_thread] [INFO ][Shadow][lu] Shadow UPDATE of MusicPlayer was ACCEPTED.
41 33170 [AWS-RemoteCtrl] AWS Remote Control Demo initialized.
42 33170 [AWS-RemoteCtrl] Use mobile application to control the remote device.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
