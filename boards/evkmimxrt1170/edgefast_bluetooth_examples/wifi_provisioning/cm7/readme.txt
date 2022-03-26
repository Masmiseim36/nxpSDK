Overview
========
This example demonstrates how the Wi-Fi of the board can be configred by Android mobile application, and publish the Wi-Fi AP information to the AWS IoT.
AWS Wi-Fi provisioning example will start advertising if the Wi-Fi AP is not configured and wait the Wi-Fi AP configuration. After connected to the Android APK,
the example will execute the request from cellphone and reply the response. When the Wi-Fi AP is configured, the Shadow demo will connect to the AWS via Wi-Fi and 
publish the configured Wi-Fi AP information.
Note: This example could NOT function with the default setting provided in SDK package because an AWS account is mandatory to run to the example, 
the end users must create their owner AWS account and configure the IoT Console before the functionality of the example could be used. Also, 
some information specified by the end customers, like Thing name, broker endpoint, etc., must be updated accordingly before the example would work. 
Check "Prepare the Demo" to get the detailed guidance of the configuration steps. 
To make sure the NVM area have clean, all flash sector need be erased before download demo code.


Toolchain supported
===================
- MCUXpresso  11.5.0
- IAR embedded Workbench  9.20.2
- Keil MDK  5.36
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
Before running the example it is need to configure AWS IoT Console and update some of project files:

1. Create AWS Account: https://console.aws.amazon.com/console/home

2. To create an AWS IOT policy based on the guide : https://docs.aws.amazon.com/freertos/latest/userguide/freertos-prereqs.html.

a. To create an IAM policy, you need to know your AWS Region and AWS account number.
   To find your AWS account number, open the AWS Management Console (https://console.aws.amazon.com/), locate and expand the menu beneath your account name in the upper-right corner, and choose "My Account". Your account ID is displayed under "Account Settings".
   To find the AWS region for your AWS account, use the AWS Command Line Interface. To install the AWS CLI, follow the instructions in the AWS Command Line Interface User Guide(https://docs.aws.amazon.com/cli/latest/userguide/cli-chap-install.html). After you install the AWS CLI, open a command prompt window and enter the following command:
   aws iot describe-endpoint
   The output should look like this:
   {
    "endpointAddress": "xxxxxxxxxxxxxx.iot.us-west-2.amazonaws.com"
   }
   In this example, the region is us-west-2.
   If the AWS Region is not set, please set the region in AWS IoT console(https://console.aws.amazon.com/iotv2/).
b. Browse to the AWS IoT console(https://console.aws.amazon.com/iotv2/).
c. In the navigation pane, choose "Secure", choose "Policies", and then choose "Create".
d. Enter a name to identify your policy. For example, the policy name is "aws_wifi_provisioning_policy".
e. In the Add statements section, choose "Advanced mode". Copy and paste the following JSON into the policy editor window.
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
c. Enter a name for your device, and then choose "Next". For example, the thing name is "aws_wifi_provisioning". Note, the max length of thing name cannot more than 23.
d. If you are connecting your microcontroller to the cloud through a mobile device, choose "Create thing without certificate". Because the Mobile SDKs use Amazon Cognito for device authentication, you do not need to create a device certificate for demos that use Bluetooth Low Energy. If you are connecting your microcontroller to the cloud directly over Wi-Fi, choose "Create certificate", choose "Activate", and then download the thing's certificate, public key, and private key. 
Due to the microcontroller connecting AWS cloud directly over Wi-Fi in the example, choose choose "Create certificate", choose "Activate", and then "download" the thing's certificate, public key, and private key.
e. Choose "Attach a policy" to attach a policy to your certificate that grants your device access to AWS IoT operations.
f. Choose the policy you just created and choose Register thing. Such as, choose the policy "aws_wifi_provisioning_policy" created in step 2.d. 
g. Choose the thing that you just created from the list of registered things, and then choose "Interact" from your thing's page. Make a note of the AWS IoT REST API endpoint.

4. To configure your AWS IoT endpoint based on the guide: https://docs.aws.amazon.com/freertos/latest/userguide/freertos-prereqs.html
You need to provide FreeRTOS with your AWS IoT endpoint so the application running on your board can send requests to the correct endpoint.
a. Browse to the AWS IoT console(https://console.aws.amazon.com/iotv2/).
b. In the navigation pane, choose "Settings".
   Your AWS IoT endpoint is displayed in "Endpoint". It should look like 1234567890123-ats.iot.us-west-2.amazonaws.com. Make a note of this endpoint.
c. In the navigation pane, choose "Manage", and then choose "Things". Your device should have an AWS IoT thing name. Make a note of this name. Such as, the thing name "aws_wifi_provisioning" created in  step 3.c.
d. Open <MCUXpresso SDK>/rtos/freertos/demos/include/aws_clientcredential.h.
e. Specify values for the following constants:
   o #define clientcredentialMQTT_BROKER_ENDPOINT "Your AWS IoT endpoint"
   o #define clientcredentialIOT_THING_NAME "The AWS IoT thing name of your board"

5. To format your AWS IoT credentials based on the guide: https://docs.aws.amazon.com/freertos/latest/userguide/freertos-prereqs.html
FreeRTOS needs the AWS IoT certificate and private keys associated with your registered thing and its permissions policies to successfully communicate with AWS IoT on behalf of your device.
FreeRTOS is a C language project, and the certificate and private key must be specially formatted to be added to the project.
a. In a browser window, open <MCUXpresso SDK>/rtos/freertos/tools/certificate_configuration/CertificateConfigurator.html.
b. Under "Certificate PEM file", choose the ID-certificate.pem.crt that you downloaded from the AWS IoT console.
c. Under "Private Key PEM file", choose the ID-private.pem.key that you downloaded from the AWS IoT console.
d. Choose "Generate and save aws_clientcredential_keys.h", and then save the file in <MCUXpresso SDK>/rtos/freertos/demos/include. This overwrites the existing file in the directory.
Note, The certificate and private key are hard-coded for demonstration purposes only. Production-level applications should store these files in a secure location.

6.  Open example's project and build it.

7.  Connect a USB cable between the PC host and the OpenSDA USB port on the target board.

8.  Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control

9.  Download the program to the target board.

10.  Either press the reset button on your board or launch the debugger in your IDE to begin running the example.


Prepare the Android application
The Android application requires Cognito service to authorize to AWS IoT in order to access device shadows. Use Amazon Cognito to create a new user pool and identity pool based on the guide: https://docs.aws.amazon.com/freertos/latest/userguide/ble-demo.html

1. To create an Amazon Cognito user pool

a. Open the Amazon Cognito console, and choose "Manage User Pools".
b. Choose "Create a user pool".
c. Give the user pool a name, and then choose "Review defaults". Such as the pool name is "aws_wifi_provisioning_user_pool".
d. From the navigation pane, choose "App clients", and then choose "Add an app client".
e. Enter a name for the app client, and then choose "Create app client". Such as the app clinet name is "aws_wifi_provisioning_apk".
f. From the navigation pane, choose "Review", and then choose "Create pool".
g. Make a note of the pool ID that appears on the "General Settings" page of your user pool.
h. From the navigation pane, choose "App clients", and then choose "Show details". Make a note of the app client ID and app client secret.

2. To create an Amazon Cognito identity pool

a. Open the Amazon Cognito console, and choose "Manage Identity Pools".
b. Enter a name for your identity pool. Such as the pool name is "aws_wifi_provisioning_identity_pool".
c. Expand "Unauthenticated identities", choose the "Enable access to unauthenticated identities". The step is used for the "mqttOn" disabled case.
d. Expand "Authentication providers", choose the "Cognito" tab, and then enter your user pool ID and app client ID as step 1.h noted.
e. Choose "Create Pool".
f. Expand "View Details", and make a note of the two IAM role names. Choose "Allow" to create the IAM roles for authenticated and unauthenticated identities to access Amazon Cognito.
g. Choose "Edit identity pool". Make a note of the identity pool ID. It should be of the form us-west-2:12345678-1234-1234-1234-123456789012.

3. To create and attach an IAM policy to the authenticated identity.

a. Open the IAM console, and from the navigation pane, choose "Roles".
b. Find and choose your authenticated identity's role, choose "Permissions policies", and then choose "Add inline policy".
c. Choose the "JSON" tab, and paste the following JSON:
{
   "Version":"2012-10-17",
   "Statement":[
      {
         "Effect":"Allow",
         "Action":[
            "iot:AttachPolicy",
            "iot:AttachPrincipalPolicy",
            "iot:Connect",
            "iot:Publish",
            "iot:Subscribe",
            "iot:Receive",
            "iot:GetThingShadow",
            "iot:UpdateThingShadow",
            "iot:DeleteThingShadow"
         ],
         "Resource":[
            "*"
         ]
      }
   ]
}
d. Choose "Review policy", enter a name for the policy, and then choose "Create policy".
e. The same steps for unauthenticated identity's role.

    More information on Amazon Cognito can be found here: https://docs.aws.amazon.com/cognito/latest/developerguide/cognito-getting-started.html
    More information on AWS IAM roles and policies can be found here: http://docs.aws.amazon.com/IAM/latest/UserGuide/access_policies_manage.html
    More information on AWS IoT policies can be found here: http://docs.aws.amazon.com/iot/latest/developerguide/authorization.html

4.  Prepare "aws_wifi_provisioning.properties" file with yours AWS credentials. It's structure looks like this:

    customer_specific_endpoint=<REST API ENDPOINT>
    cognito_pool_id=<COGNITO POOL ID>
    thing_name=<THING NAME>
    region=<REGION>
    policy_name=<POLICY>

    For example:
    customer_specific_endpoint=xxxxxx-ats.iot.us-west-2.amazonaws.com
    cognito_pool_id=us-west-2:5xxxx7-3xxxx9-4xxx5-axxf-0xxxxxxxxxxb
    thing_name=aws_wifi_provisioning
    region=us-west-2
    policy_name=aws_wifi_provisioning_policy

    Then move properties file into your Android device (application will ask for properties file though file browser dialog during first run).

5.  To run Android application as the following, 
    o Update the configuration. Please refer to the documents of FreeRTOS BLE Mobile SDK for Android (<MCUXpresso SDK>\boards\<board name>\edgefast_bluetooth_examples\wifi_provisioning\amazon-freertos-ble-android-sdk\README.md and <MCUXpresso SDK>\boards\<board name>\edgefast_bluetooth_examples\wifi_provisioning\amazon-freertos-ble-android-sdk\app\README.md) for porject setting firstly.
    o Open project in Android Studio, build it, attach Android device and Run application.
    Please refer <MCUXpresso SDK>\boards\<board name>\edgefast_bluetooth_examples\wifi_provisioning\amazon-freertos-ble-android-sdk\documentation for more.

Application requires at least Android Android 6.0 (API level 23) or higher, and Bluetooth 4.2 or higher.

Please refer the following steps for detail,
a. If you don't need the mqtt proxy feature, set boolean `mqttOn` to `false` in `DeviceScanFragment.java` and `WifiAwsFragment.java`, then skip to step c.
b. Replace the constants in `DemoConstants.java` and `res/raw/awsconfiguration.json`.
c. Build the app in Android Studio, after enabling "USB debugging" on your Android device.
d. Run the demo app. Register account and sign-in if sign-in page appears.
e. In the running application, when asked, select aws_wifi_provisioning.properties file with AWS IoT preferences (step 4 mentioned).
f. Select "WIFI PROVISIONING".
g. Click on the "SCAN FOR BLE DEVICES" button to scan for nearby BLE devices.
h. Toggle the switch to connect to one of them. "More..." menu will be enabled after connection is successful.
i. Click on the "More..." menu, and then choose "WiFi Provisioning" to start aws wifi provisioning example experience. Please note only "WiFi Provisioning" is sopported in "More..." menu.
j. Select a proper Wi-Fi AP from Wi-Fi AP list, and input the passkey to set the Wi-Fi AP.

Running the demo
================
The debug console is used to show the status of the example. Please open the debug console in the begining. The log below shows the output of the example in the terminal window. The log can be different based on your Wi-Fi network configuration and based on the actions, which you have done in the Android application.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
0 2 [main_task] [INFO ][MSD_FATFS][0] USB Host stack successfully initialized
1 75 [main_task] Write certificate...
2 100 [iot_thread] [INFO ][DEMO][0] ---------STARTING DEMO---------

3 101 [iot_thread] [INFO ][INIT][0] SDK successfully initialized.
MAC Address: D8:C0:A6:C0:B0:4B 
[net] Initialized TCP/IP networking stack
[wm_wlan] WLAN_REASON_INITIALIZED

4 4321 [EtherMind RD Ta] [INFO ][IOT_BLE_HAL_COMMON_GAP][0] Bluetooth ON Initialization Completed.
5 4321 [EtherMind RD Ta] [INFO ][IOT_BLE_HAL_COMMON_GAP][539099592] Stack Version - 016.002.000.
6 4332 [iot_thread] [INFO ][DEMO][0] No networks connected for the demo. Waiting for a network connection. 
7 6474 [EtherMind RD Ta] [INFO ][DEMO][0] BLE Connected to remote device, connId = 128

8 9301 [EtherMind RD Ta] [WARN ][BLE_WIFI_PROV][0] Networks 20 exceeds configured value, truncating to  10 max network

[wm_wlan] Connecting to NXPOPEN .....

9 28802 [EtherMind RD Ta] [INFO ][BLE_WIFI_PROV][0] Connect flag not set in request, using default value always connect = 1
[wm_wlan] Connected to with IP = [192.168.1.109]

10 55658 [iot_thread] [INFO ][DEMO][0] Successfully initialized the demo. Network type for the demo: 1
11 55658 [iot_thread] [INFO ][MQTT][539099592] MQTT library successfully initialized.
12 55659 [iot_thread] [INFO ][Shadow][0] Shadow library successfully initialized.
13 55660 [iot_thread] [INFO ][DEMO][0] Shadow Thing Name is aws_wifi_provisioning (length 21).
14 60662 [iot_thread] [INFO ][MQTT][0] Establishing new MQTT connection.
15 60663 [iot_thread] [INFO ][MQTT][0] Anonymous metrics (SDK language, SDK version) will be provided to AWS IoT. Recompile with AWS_IOT_MQTT_ENABLE_METRICS set to 0 to disable.
16 60665 [iot_thread] [INFO ][MQTT][0] (MQTT connection 20231238, CONNECT operation 20230a30) Waiting for operation completion.
17 60959 [iot_thread] [INFO ][MQTT][0] (MQTT connection 20231238, CONNECT operation 20230a30) Wait complete with result SUCCESS.
18 60960 [iot_thread] [INFO ][MQTT][539099592] New MQTT connection 20222884 established.
19 60960 [iot_thread] [INFO ][Shadow][539099592] (aws_wifi_provisioning) Modifying Shadow DELTA callback.
20 60960 [iot_thread] [INFO ][Shadow][539099592] (aws_wifi_provisioning) Adding new DELTA callback.
21 60961 [iot_thread] [INFO ][MQTT][539099592] (MQTT connection 20231238) SUBSCRIBE operation scheduled.
22 60961 [iot_thread] [INFO ][MQTT][539099592] (MQTT connection 20231238, SUBSCRIBE operation 20231188) Waiting for operation completion.
23 61226 [iot_thread] [INFO ][MQTT][0] (MQTT connection 20231238, SUBSCRIBE operation 20231188) Wait complete with result SUCCESS.
24 61226 [iot_thread] [INFO ][Shadow][0] (aws_wifi_provisioning) Shadow DELTA callback operation complete with result SUCCESS.
25 61226 [iot_thread] [INFO ][Shadow][0] (aws_wifi_provisioning) Modifying Shadow UPDATED callback.
26 61227 [iot_thread] [INFO ][Shadow][539099592] (aws_wifi_provisioning) Adding new UPDATED callback.
27 61230 [iot_thread] [INFO ][MQTT][0] (MQTT connection 20231238) SUBSCRIBE operation scheduled.
28 61230 [iot_thread] [INFO ][MQTT][539099592] (MQTT connection 20231238, SUBSCRIBE operation 202328d8) Waiting for operation completion.
29 61495 [iot_thread] [INFO ][MQTT][0] (MQTT connection 20231238, SUBSCRIBE operation 202328d8) Wait complete with result SUCCESS.
30 61495 [iot_thread] [INFO ][Shadow][0] (aws_wifi_provisioning) Shadow UPDATED callback operation complete with result SUCCESS.
31 61496 [iot_thread] [INFO ][MQTT][539099592] (MQTT connection 20231238) SUBSCRIBE operation scheduled.
32 61498 [iot_thread] [INFO ][MQTT][539099592] (MQTT connection 20231238, SUBSCRIBE operation 202327d0) Waiting for operation completion.
33 61750 [iot_thread] [INFO ][MQTT][0] (MQTT connection 20231238, SUBSCRIBE operation 202327d0) Wait complete with result SUCCESS.
34 61751 [iot_thread] [INFO ][MQTT][539099592] (MQTT connection 20231238) SUBSCRIBE operation scheduled.
35 61751 [iot_thread] [INFO ][MQTT][539099592] (MQTT connection 20231238, SUBSCRIBE operation 202326c8) Waiting for operation completion.
36 62013 [iot_thread] [INFO ][MQTT][0] (MQTT connection 20231238, SUBSCRIBE operation 202326c8) Wait complete with result SUCCESS.
37 62016 [iot_thread] [INFO ][MQTT][0] (MQTT connection 20231238) MQTT PUBLISH operation queued.
38 62285 [iot_thread] [WARN ][Shadow][0] Shadow DELETE of aws_wifi_provisioning was REJECTED.
39 62285 [iot_thread] [WARN ][Shadow][539099592] Code 404: "No shadow exists with name: 'aws_wifi_provisioning'".
40 62286 [iot_thread] [INFO ][MQTT][0] (MQTT connection 20231238) UNSUBSCRIBE operation scheduled.
41 62286 [iot_thread] [INFO ][MQTT][539099592] (MQTT connection 20231238, UNSUBSCRIBE operation 202326c8) Waiting for operation completion.
42 62838 [iot_thread] [INFO ][MQTT][0] (MQTT connection 20231238, UNSUBSCRIBE operation 202326c8) Wait complete with result SUCCESS.
43 62839 [iot_thread] [INFO ][MQTT][539099592] (MQTT connection 20231238) UNSUBSCRIBE operation scheduled.
44 62839 [iot_thread] [INFO ][MQTT][539099592] (MQTT connection 20231238, UNSUBSCRIBE operation 202326c8) Waiting for operation completion.
45 63088 [iot_thread] [INFO ][MQTT][0] (MQTT connection 20231238, UNSUBSCRIBE operation 202326c8) Wait complete with result SUCCESS.
46 63089 [iot_thread] [INFO ][DEMO][0] Successfully cleared Shadow of aws_wifi_provisioning.
47 63089 [iot_thread] [INFO ][DEMO][539099592] Shadow update {"state":{"reported":{"wifi_list":{"count":0,"wifi":[]}}},"clientToken":"063089"}
48 63092 [iot_thread] [INFO ][MQTT][0] (MQTT connection 20231238) SUBSCRIBE operation scheduled.
49 63092 [iot_thread] [INFO ][MQTT][539099592] (MQTT connection 20231238, SUBSCRIBE operation 202326c8) Waiting for operation completion.
50 63399 [iot_thread] [INFO ][MQTT][0] (MQTT connection 20231238, SUBSCRIBE operation 202326c8) Wait complete with result SUCCESS.
51 63400 [iot_thread] [INFO ][MQTT][539099592] (MQTT connection 20231238) SUBSCRIBE operation scheduled.
52 63400 [iot_thread] [INFO ][MQTT][539099592] (MQTT connection 20231238, SUBSCRIBE operation 202326c8) Waiting for operation completion.
53 63667 [iot_thread] [INFO ][MQTT][0] (MQTT connection 20231238, SUBSCRIBE operation 202326c8) Wait complete with result SUCCESS.
54 63670 [iot_thread] [INFO ][MQTT][0] (MQTT connection 20231238) MQTT PUBLISH operation queued.
55 63943 [iot_thread] [INFO ][DEMO][0] Shadow was updated!
Previous: {"state":{}}
Current:  {"state":{"reported":{"wifi_list":{"count":0,"wifi":[]}}}}
56 63954 [iot_thread] [INFO ][Shadow][0] Shadow UPDATE of aws_wifi_provisioning was ACCEPTED.
57 63955 [iot_thread] [INFO ][DEMO][0] Successfully sent Shadow update.
58 64955 [iot_thread] [INFO ][DEMO][0] Saved wifi config: ssid NXPOPEN, index 0
59 64955 [iot_thread] [INFO ][DEMO][539099592] Shadow update {"state":{"reported":{"wifi_list":{"count":1,"wifi":[{"ssid":"NXPOPEN","bssid":"A46C2A46CB41","security":3}]}}},"clientToken":"064955"}
60 64959 [iot_thread] [INFO ][MQTT][0] (MQTT connection 20231238) MQTT PUBLISH operation queued.
61 65230 [iot_thread] [INFO ][DEMO][0] Shadow was updated!
Previous: {"state":{"reported":{"wifi_list":{"count":0,"wifi":[]}}}}
Current:  {"state":{"reported":{"wifi_list":{"count":1,"wifi":[{"ssid":"NXPOPEN","bssid":"A46C2A46CB41","security":3}]}}}}62 65241 [iot_thread] [INFO ][Shadow][0] Shadow UPDATE of aws_wifi_provisioning was ACCEPTED.
63 65242 [iot_thread] [INFO ][DEMO][0] Successfully sent Shadow update.
64 65243 [iot_thread] [INFO ][MQTT][0] (MQTT connection 20231238) SUBSCRIBE operation scheduled.
65 65244 [iot_thread] [INFO ][MQTT][539099592] (MQTT connection 20231238, SUBSCRIBE operation 202326c8) Waiting for operation completion.
66 65712 [iot_thread] [INFO ][MQTT][0] (MQTT connection 20231238, SUBSCRIBE operation 202326c8) Wait complete with result SUCCESS.
67 65713 [iot_thread] [INFO ][MQTT][539099592] (MQTT connection 20231238) SUBSCRIBE operation scheduled.
68 65713 [iot_thread] [INFO ][MQTT][539099592] (MQTT connection 20231238, SUBSCRIBE operation 202326c8) Waiting for operation completion.
69 65969 [iot_thread] [INFO ][MQTT][0] (MQTT connection 20231238, SUBSCRIBE operation 202326c8) Wait complete with result SUCCESS.
70 65971 [iot_thread] [INFO ][MQTT][0] (MQTT connection 20231238) MQTT PUBLISH operation queued.
71 66281 [iot_thread] [INFO ][Shadow][0] Shadow DELETE of aws_wifi_provisioning was ACCEPTED.
72 66282 [iot_thread] [INFO ][MQTT][0] (MQTT connection 20231238) UNSUBSCRIBE operation scheduled.
73 66282 [iot_thread] [INFO ][MQTT][539099592] (MQTT connection 20231238, UNSUBSCRIBE operation 202326c8) Waiting for operation completion.
74 66759 [iot_thread] [INFO ][MQTT][0] (MQTT connection 20231238, UNSUBSCRIBE operation 202326c8) Wait complete with result SUCCESS.
75 66760 [iot_thread] [INFO ][MQTT][539099592] (MQTT connection 20231238) UNSUBSCRIBE operation scheduled.
76 66760 [iot_thread] [INFO ][MQTT][539099592] (MQTT connection 20231238, UNSUBSCRIBE operation 202326c8) Waiting for operation completion.
77 67013 [iot_thread] [INFO ][MQTT][0] (MQTT connection 20231238, UNSUBSCRIBE operation 202326c8) Wait complete with result SUCCESS.
78 67014 [iot_thread] [INFO ][DEMO][0] Successfully cleared Shadow of aws_wifi_provisioning.
79 67014 [iot_thread] [INFO ][MQTT][539099592] (MQTT connection 20231238) Disconnecting connection.
80 67017 [iot_thread] [INFO ][MQTT][0] (MQTT connection 20231238, DISCONNECT operation 20231130) Waiting for operation completion.
81 67018 [iot_thread] [INFO ][MQTT][0] (MQTT connection 20231238, DISCONNECT operation 20231130) Wait complete with result SUCCESS.
82 67018 [iot_thread] [INFO ][MQTT][539099592] (MQTT connection 20231238) Connection disconnected.
83 67019 [iot_thread] [INFO ][MQTT][539099592] (MQTT connection 20231238) Network connection closed.
84 67266 [iot_thread] [INFO ][MQTT][0] (MQTT connection 20231238) Network connection destroyed.
85 67266 [iot_thread] [INFO ][Shadow][539099592] Shadow library cleanup done.
86 67267 [iot_thread] [INFO ][MQTT][0] MQTT library cleanup done.
87 67267 [iot_thread] [INFO ][DEMO][539099592] Demo completed successfully.
[wm_wlan] Dis-connected

88 67320 [iot_thread] [INFO ][INIT][0] SDK cleanup done.
89 67320 [iot_thread] [INFO ][DEMO][539099592] -------DEMO FINISHED-------

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

There are some limitations for the example,
1. Only one wifi network configuration could be saved. Please view the ticket https://github.com/aws/amazon-freertos/issues/2678 for detail information.
2. The building warnings on IAR and arm gcc tool-chain. Please view the ticket https://github.com/aws/amazon-freertos/issues/2638 for detail information.
3. Once the wifi network is configured, the BLE will be disconnected and then disabled. So the BLE connection could not be established after the WIFI configured successfully.
4. If the log "[wm_wlan] Connection Failed !" following the 3 times log "[wm_wlan] Connect to AP FAIL ! Retrying .....", it means the wifi network is not established. Please check whether the entered passkey is correct, whether WIFI AP is working, or whether the example is blocked by blacklist or whitelist. And then retry again.
5. If the blank screen is shown when start running the APK, it maybe the network issue. Please view the ticket https://github.com/aws/amazon-freertos-ble-android-sdk/issues/34 for detail information.
