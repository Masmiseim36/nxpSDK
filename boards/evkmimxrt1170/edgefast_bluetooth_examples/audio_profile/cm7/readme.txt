Overview
========
This demo demonstrates the follow function:
1. There are five parts working in the demo: AWS cloud, Android app, audio demo (running on target board), U-disk and BlueTooth headset.
2. With an app running on the smart phone (Android phone), the end users could connect to AWS cloud and control the audio demo running on the target board EVK board through AWS cloud. Some operations like play, play next, pause, etc., could be used to control the media play functionalities. 
3. Audio demo running on the target board EVK board connects to the AWS through Wifi, also a connection could be established between the target board EVK board and a Bluetooth headset. To get the media resource (mp3 files) from the U-disk, a HS USB host is enabled, and a U-disk with mp3 files should be connected to target board EVK board via the USB port. After that, the audio demo will search the root directory of U-disk for the music files (now only mp3 files are supported), and upload the song file list to AWS, then the song list would be shown in the app running on the smart phone. Finally, the music could be played out via the Bluetooth headset once end user controls the app to play the mp3 file.
Note:
1. This demo could NOT function with the default setting provided in SDK package because a AWS account is mandatory to run to the demo, the end users must create their owner AWS account and configure the IoT Thing before the functionality of the demo could be used. Also some information specified by the end customers, like Thing name, Wifi SSID Wifi password, etc., must be updated accordingly before the demo would work. Check (Prepare the Demo) to get the detailed guidance of the configuration steps.
2. The music files names in U-disk need be english.
3. The volume of audio adjustment is not supported.


Toolchain supported
===================
- MCUXpresso  11.5.0
- IAR embedded Workbench  9.20.2
- GCC ARM Embedded  10.3.1

Hardware requirements
=====================
- Micro USB cable
- evkmimxrt1170 board
- Personal Computer
- One of the following modules:
  - AzureWave AW-AM510-M.2
  - AzureWave AW-CM358-M.2
  - AzureWave AW-AM457-uSD
  - Murata uSD-M.2 Adapter (LBEE0ZZ1WE-uSD-M2) and Embedded Artists 1XK M.2 Module (EAR00385)
  - Embedded Artists 1XK M.2 Module (EAR00385) - direct M2 connection.
  - Murata uSD-M.2 Adapter (LBEE0ZZ1WE-uSD-M2) and Embedded Artists 1ZM M.2 Module (EAR00364)
  - Embedded Artists 1ZM M.2 Module (EAR00364) - direct M2 connection.

Board settings
==============
Before building the example application select Wi-Fi module macro in the app_config.h. (see #define WIFI_<SoC Name>_BOARD_<Module Name>).
If you want to use AzureWave WIFI_IW416_BOARD_AW_AM510MA, please change the macro to WIFI_IW416_BOARD_AW_AM510MA.
If you want to use AzureWave WIFI_88W8987_BOARD_AW_CM358MA-M.2, please change the macro to WIFI_88W8987_BOARD_AW_CM358MA.
If you want to use AzureWave WIFI_IW416_BOARD_AW_AM457_USD, please change the macro to WIFI_IW416_BOARD_AW_AM457_USD.
If you want to use Embedded Artists Type 1XK module with uSD-M2 adapter, please change the macro to WIFI_IW416_BOARD_MURATA_1XK_USD .
If you want to use Embedded Artists Type 1XK module(EAR00385 M2 only), please change the macro to WIFI_IW416_BOARD_MURATA_1XK_M2.
If you want to use Embedded Artists Type 1ZM module with uSD-M2 adapter, please change the macro to WIFI_88W8987_BOARD_MURATA_1ZM_USD.
If you want to use Embedded Artists Type 1ZM module(EAR00364 M2 only), please change the macro to WIFI_88W8987_BOARD_MURATA_1ZM_M2.

USB port: J20

Jumper settings for RT1170 (enables external 5V supply):
remove  J38 5-6
connect J38 1-2
connect J43 with external power(controlled by SW5)

Murata Solution Board settings
Murata uSD-M.2 adapter resource page: https://www.murata.com/en-us/products/connectivitymodule/wi-fi-bluetooth/overview/lineup/usd-m2-adapter
Murata uSD-M.2 adapter datasheet: https://www.murata.com/-/media/webrenewal/products/connectivitymodule/asset/pub/rfm/data/usd-m2_revb1.ashx
Embedded Artists M.2 module resource page: https://www.embeddedartists.com/m2
Embedded Artists 1XK module datasheet: https://www.embeddedartists.com/doc/ds/1XK_M2_Datasheet.pdf
Embedded Artists 1ZM module datasheet: https://www.embeddedartists.com/doc/ds/1ZM_M2_Datasheet.pdf

Jumper settings for Murata uSD-M.2 adapter:
  - Both J12 & J13 = 1-2: WLAN-SDIO = 1.8V; and BT-UART & WLAN/BT-CTRL = 3.3V
  - J1 = 2-3: 3.3V from uSD connector

The following pins between the evkmimxrt1170 board and Murata uSD-M.2 Adapter with Embedded Artists 1ZM M.2 Module or 1XK M.2 Module are connected using male-to-female jumper cables:

----------------------------------------------------------------------------------------------
PIN NAME         | uSD-M.2 Adapter |   I.MXRT1170    | PIN NAME OF RT1170 | GPIO NAME OF RT1170
----------------------------------------------------------------------------------------------
BT_UART_TXD_HOST |  J9(pin 1)  	   |   J25(pin 13)   |    LPUART7_RXD     | GPIO_AD_01
BT_UART_RXD_HOST |  J9(pin 2)  	   |   J25(pin 15)   |    LPUART7_TXD     | GPIO_AD_00
BT_UART_RTS_HOST |  J8(pin 3)  	   |   J25(pin 11)   |    LPUART7_CTS     | GPIO_AD_02
BT_UART_CTS_HOST |  J8(pin 4)  	   |   J25(pin 9)    |    LPUART7_RTS     | GPIO_AD_03
----------------------------------------------------------------------------------------------

NOTE: With direct M2 connection (EAR00364/EAR00385) no additional cabling is needed.

AzureWave Solution Board settings
Jumper settings for AzureWave AW-AM457-uSD Module:
  - J11 2-3: VIO_SD 3.3V (Voltage level of SDIO pins is 3.3V)
  - J2  1-2: 3.3V VIO_uSD (Power Supply from uSD connector)
  - J4  2-3: 3.3V VIO

The hardware should be reworked according to the Hardware Rework Guide for MIMXRT1170-EVK and AW-AM457-uSD in document Hardware Rework Guide for EdgeFast BT PAL.
The pin connect for UART HCI as the following table,
------------------------------------------------------------------------------------
PIN NAME | AW-AM457-USD |   I.MXRT1170  | PIN NAME OF RT1170 | GPIO NAME OF RT1170
------------------------------------------------------------------------------------
UART_TXD |  J10(pin 4)  |   J25(pin 13)   |    LPUART7_RXD     | GPIO_AD_01
UART_RXD |  J10(pin 2)  |   J25(pin 15)   |    LPUART7_TXD     | GPIO_AD_00
UART_CTS |  J10(pin 8)  |   J25(pin 9)    |    LPUART7_RTS     | GPIO_AD_03
UART_RTS |  J10(pin 6)  |   J25(pin 11)   |    LPUART7_CTS     | GPIO_AD_02
GND      |  J6(pin 7)   |   J26(pin 1)    |    GND             | GND
------------------------------------------------------------------------------------

The hardware should be reworked according to the Hardware Rework Guide for MIMXRT1170-EVK and AW-CM358MA in document Hardware Rework Guide for EdgeFast BT PAL.

The hardware should be reworked according to the Hardware Rework Guide for MIMXRT1170-EVK and AW-CM510MA in document Hardware Rework Guide for EdgeFast BT PAL.

Note:
After downloaded binary into qspiflash and boot from qspiflash directly,
please reset the board by pressing SW4 or power off and on the board to run the application.
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

8. Connect U-disk to USB port on the target board through the USB OTG cable.

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
   a) Install and run pre-build apk on Android device (<MCUXpresso SDK>\boards\<board name>\edgefast_bluetooth_examples\audio_pofile\android_app\AwsMusicControl.apk)
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
0 154 [main_task] Write certificate...

 

1 221 [iot_thread] [INFO ][DEMO][539114128] ---------STARTING DEMO---------

 


2 222 [iot_thread] [INFO ][INIT][539114128] SDK successfully initialized.

 

MAC Address: 80:D2:1D:E8:2F:67 
[net] Initialized TCP/IP networking stack
3 3571 [iot_thread] Connecting to nxp .....

 

4 15602 [wlcmgr] Connected to with IP = [192.168.199.68]

 

5 15622 [iot_thread] [INFO ][DEMO][0] Successfully initialized the demo. Network type for the demo: 1

 

6 15622 [iot_thread] [INFO] Create a TCP connection to a2nxzv2h17k05v.ats.iot.cn-north-1.amazonaws.com.cn:8883.
7 22785 [iot_thread] [INFO] Packet received. ReceivedBytes=2.
8 22785 [iot_thread] [INFO] CONNACK session present bit not set.
9 22785 [iot_thread] [INFO] Connection accepted.
10 22785 [iot_thread] [INFO] Received MQTT CONNACK successfully from broker.
11 22785 [iot_thread] [INFO] MQTT connection established with the broker.
12 22785 [iot_thread] [INFO] MQTT connection successfully established with broker.

 


13 22785 [iot_thread] [INFO] A clean MQTT connection is established. Cleaning up all the stored outgoing publishes.

 


14 22786 [iot_thread] [INFO] SUBSCRIBE topic $aws/things/aws_wifi_provisioning/shadow/delete/accepted to broker.

 


15 22938 [iot_thread] [INFO] Packet received. ReceivedBytes=3.
16 22938 [iot_thread] [INFO] MQTT_PACKET_TYPE_SUBACK.

 


17 23341 [iot_thread] [INFO] SUBSCRIBE topic $aws/things/aws_wifi_provisioning/shadow/delete/rejected to broker.

 


18 23501 [iot_thread] [INFO] Packet received. ReceivedBytes=3.
19 23502 [iot_thread] [INFO] MQTT_PACKET_TYPE_SUBACK.

 


20 23904 [iot_thread] [INFO] the published payload: 
 
21 23905 [iot_thread] [INFO] PUBLISH sent for topic $aws/things/aws_wifi_provisioning/shadow/delete to broker with packet ID 3.

 


22 24027 [iot_thread] [INFO] Packet received. ReceivedBytes=2.
23 24027 [iot_thread] [INFO] Ack packet deserialized with result: MQTTSuccess.
24 24027 [iot_thread] [INFO] State record updated. New state=MQTTPublishDone.
25 24028 [iot_thread] [INFO] PUBACK received for packet id 3.

 


26 24028 [iot_thread] [INFO] Cleaned up outgoing publish packet with packet id 3.

 


27 24095 [iot_thread] [INFO] Packet received. ReceivedBytes=99.
28 24095 [iot_thread] [INFO] De-serialized incoming PUBLISH packet: DeserializerResult=MQTTSuccess.
29 24095 [iot_thread] [INFO] State record updated. New state=MQTTPubAckSend.
30 24095 [iot_thread] [INFO] pPublishInfo->pTopicName:$aws/things/aws_wifi_provisioning/shadow/delete/accepted.
31 24095 [iot_thread] [INFO] Received an MQTT incoming publish on /delete/accepted topic.
32 24499 [iot_thread] [INFO] UNSUBSCRIBE sent topic $aws/things/aws_wifi_provisioning/shadow/delete/accepted to broker.

 


33 24671 [iot_thread] [INFO] Packet received. ReceivedBytes=2.
34 24671 [iot_thread] [INFO] MQTT_PACKET_TYPE_UNSUBACK.

 


35 25075 [iot_thread] [INFO] UNSUBSCRIBE sent topic $aws/things/aws_wifi_provisioning/shadow/delete/rejected to broker.

 


36 25243 [iot_thread] [INFO] Packet received. ReceivedBytes=2.
37 25243 [iot_thread] [INFO] MQTT_PACKET_TYPE_UNSUBACK.

 


38 25646 [iot_thread] [INFO] SUBSCRIBE topic $aws/things/aws_wifi_provisioning/shadow/update/delta to broker.

 


39 25780 [iot_thread] [INFO] Packet received. ReceivedBytes=3.
40 25780 [iot_thread] [INFO] MQTT_PACKET_TYPE_SUBACK.

 


41 26182 [iot_thread] [INFO] the published payload:{"state":{"desired":{"playState":false,"playIndex":0},"reported":{"playState":false,"playIndex":0,"playerReady":false,"musicList":[]}},"clientToken": "token-26182"} 
 
42 26184 [iot_thread] [INFO] PUBLISH sent for topic $aws/things/aws_wifi_provisioning/shadow/update to broker with packet ID 7.

 


43 26445 [iot_thread] [INFO] Packet received. ReceivedBytes=2.
44 26445 [iot_thread] [INFO] Ack packet deserialized with result: MQTTSuccess.
45 26445 [iot_thread] [INFO] State record updated. New state=MQTTPublishDone.
46 26445 [iot_thread] [INFO] PUBACK received for packet id 7.

 


47 26445 [iot_thread] [INFO] Cleaned up outgoing publish packet with packet id 7.

 


48 26848 [iot_thread] [INFO] AWS Remote Control Demo initialized.
49 26848 [iot_thread] [INFO] Use mobile application to control the remote device.
Bluetooth initialized

 

Copyright  2020  NXP

 

>> 

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
