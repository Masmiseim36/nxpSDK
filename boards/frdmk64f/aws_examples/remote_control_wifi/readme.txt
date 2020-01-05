Overview
========
This example demonstrates how the board can be controlled by Android mobile application with usage the AWS IoT.
User LEDs on the board can be controlled and also an action can be invoked to read data from accelerometer and report it to AWS IoT device shadow.
Please take look into section "Board settings" if there are some board limitations to control LEDs or read data from accelerometer.


Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- GCC ARM Embedded  8.3.1
- Keil MDK  5.29
- MCUXpresso  11.1.0

Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-K64F board
- "GT202 Adaptor V1.04" or "Silex SX-ULPAN-2401" or "FRDM-KL64Z click shield with WIFI10CLICK"
- Personal Computer

Board settings
==============
"Plug SX-ULPAN-2401" or "GT202 Adaptor" or "WIFI10CLICK" board to FRDM stackable headers (J1, J2, J3, J4).
GT202 shield is compatible with FRDM-K64 SCH-REV E and higher.
Previous revisions cannot provide GPIO mux on J2:2 (WLAN_PWRON).

The default compilation settings are predefined for WIFI10CLICK shield.
If you want to use "GT202 shield" or "Silex SX-ULPAN-2401", please add following "Compiler define" in project options:

WIFISHIELD_IS=WIFISHIELD_IS_GT202
WIFISHIELD_IS=WIFISHIELD_IS_SILEX2401

The "wifi_shield.h" then includes specific shield support.
Prepare the Demo
================
Please update WiFi firmware to version 3.3.6 using "qca_fwupdate" demo.
Before running the demo it is need to configure AWS IoT Console and update some of project files:

1.  Create AWS Account: https://console.aws.amazon.com/console/home

2.  Configure device in the AWS IoT Console base on this guide: https://docs.aws.amazon.com/iot/latest/developerguide/iot-sdk-setup.html

    Make note of example's "Thing name" and "REST API endpoint". These strings need to be set in the "aws_clientcredential.h".

    Example:
        static const char clientcredentialMQTT_BROKER_ENDPOINT[] = "abcdefgh123456.iot.us-west-2.amazonaws.com";
        #define clientcredentialIOT_THING_NAME "MyExample"

    In the next step you will get the "device certificate" and the "primary key". The device certificate and private key needs to be opened in text editor and its content copied into the "aws_clientcredential_keys.h".
    Or you can use the CertificateConfigurator.html (mcu-sdk-2.0\rtos\amazon-freertos\tools\certificate_configuration) to generate the "aws_clientcredential_keys.h".

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

3.  This demo needs WiFi network with internet access.
    Update these macros in "aws_clientcredential.h" based on your WiFi network configuration:
        #define clientcredentialWIFI_SSID       "Paste WiFi SSID here."
        #define clientcredentialWIFI_PASSWORD   "Paste WiFi password here."

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
The log below shows the output of the demo in the terminal window. The log can be different based on your WiFi network configuration and based on the actions, which you have done in the Android application.

Android application displays Accelerometer data for all three axis and status of LEDs which is split into Desired and Reported section. Desired value is value wanted by user and Reported value is actual value of LEDs on device.

- When you turn on/off some of RGB LED in the Android application, the change should be visible on RGB LED on the board.
- When you refresh accelerometer data on Android application, you should see changes in the accelerometer data.

Every mentioned action takes approximately 1-3 seconds.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
0 160 [Tmr Svc] Write certificate...
1 463 [Tmr Svc] Device credential provisioning succeeded.
2 463 [Tmr Svc] Starting WiFi...
3 1718 [Tmr Svc] WiFi module initialized.
4 7731 [Tmr Svc] WiFi connected to AP Guest.
5 7731 [Tmr Svc] IP Address acquired 192.168.2.101
6 7740 [AWS-RemoteCtrl] [INFO ][INIT][7740] SDK successfully initialized.
7 7747 [AWS-RemoteCtrl] [INFO ][Shadow][7747] Shadow library successfully initialized.
9 12554 [AWS-RemoteCtrl] [INFO ][MQTT][12554] Establishing new MQTT connection.
10 12562 [AWS-RemoteCtrl] [INFO ][MQTT][12562] Anonymous metrics (SDK language, SDK version) will be provided to AWS IoT. Recompile with AWS_IOT_MQTT_ENABLE_METRICS set to 0 to disable.
11 12580 [AWS-RemoteCtrl] [INFO ][MQTT][12579] (MQTT connection 20009320, CONNECT operation 200094a0) Waiting for operation completion.
12 12777 [AWS-RemoteCtrl] [INFO ][MQTT][12777] (MQTT connection 20009320, CONNECT operation 200094a0) Wait complete with result SUCCESS.
13 12789 [AWS-RemoteCtrl] [INFO ][MQTT][12789] New MQTT connection 20008900 established.
14 12797 [AWS-RemoteCtrl] [INFO ][MQTT][12797] (MQTT connection 20009320) SUBSCRIBE operation scheduled.
15 12798 [AWS-RemoteCtrl] [INFO ][MQTT][12798] (MQTT connection 20009320, SUBSCRIBE operation 2000a438) Waiting for operation completion.
16 12957 [AWS-RemoteCtrl] [INFO ][MQTT][12957] (MQTT connection 20009320, SUBSCRIBE operation 2000a438) Wait complete with result SUCCESS.
17 12957 [AWS-RemoteCtrl] [INFO ][MQTT][12957] (MQTT connection 20009320) SUBSCRIBE operation scheduled.
18 12957 [AWS-RemoteCtrl] [INFO ][MQTT][12957] (MQTT connection 20009320, SUBSCRIBE operation 2000a438) Waiting for operation completion.
19 13131 [AWS-RemoteCtrl] [INFO ][MQTT][13131] (MQTT connection 20009320, SUBSCRIBE operation 2000a438) Wait complete with result SUCCESS.
20 13144 [AWS-RemoteCtrl] [INFO ][MQTT][13144] (MQTT connection 20009320) MQTT PUBLISH operation queued.
21 13354 [iot_thread] [WARN ][Shadow][13354] Shadow DELETE of aws_rpr_michal was REJECTED.
22 13354 [iot_thread] [WARN ][Shadow][13354] Code 404: "No shadow exists with name: 'aws_rpr_michal'".
23 13371 [AWS-RemoteCtrl] [INFO ][MQTT][13371] (MQTT connection 20009320) UNSUBSCRIBE operation scheduled.
24 13372 [AWS-RemoteCtrl] [INFO ][MQTT][13372] (MQTT connection 20009320, UNSUBSCRIBE operation 2000a438) Waiting for operation completion.
25 13558 [AWS-RemoteCtrl] [INFO ][MQTT][13557] (MQTT connection 20009320, UNSUBSCRIBE operation 2000a438) Wait complete with result SUCCESS.
26 13558 [AWS-RemoteCtrl] [INFO ][MQTT][13558] (MQTT connection 20009320) UNSUBSCRIBE operation scheduled.
27 13558 [AWS-RemoteCtrl] [INFO ][MQTT][13558] (MQTT connection 20009320, UNSUBSCRIBE operation 2000a438) Waiting for operation completion.
28 13720 [AWS-RemoteCtrl] [INFO ][MQTT][13720] (MQTT connection 20009320, UNSUBSCRIBE operation 2000a438) Wait complete with result SUCCESS.
29 13733 [AWS-RemoteCtrl] [INFO ][Shadow][13733] (aws_rpr_michal) Modifying Shadow DELTA callback.
30 13742 [AWS-RemoteCtrl] [INFO ][Shadow][13741] (aws_rpr_michal) Adding new DELTA callback.
31 13750 [AWS-RemoteCtrl] [INFO ][MQTT][13750] (MQTT connection 20009320) SUBSCRIBE operation scheduled.
32 13750 [AWS-RemoteCtrl] [INFO ][MQTT][13750] (MQTT connection 20009320, SUBSCRIBE operation 20009528) Waiting for operation completion.
33 13943 [AWS-RemoteCtrl] [INFO ][MQTT][13942] (MQTT connection 20009320, SUBSCRIBE operation 20009528) Wait complete with result SUCCESS.
34 13955 [AWS-RemoteCtrl] [INFO ][Shadow][13955] (aws_rpr_michal) Shadow DELTA callback operation complete with result SUCCESS.
35 13967 [AWS-RemoteCtrl] [INFO ][Shadow][13967] (aws_rpr_michal) Modifying Shadow UPDATED callback.
36 13976 [AWS-RemoteCtrl] [INFO ][Shadow][13976] (aws_rpr_michal) Shadow UPDATED callback operation complete with result SUCCESS.
37 13988 [AWS-RemoteCtrl] [INFO ][MQTT][13988] (MQTT connection 20009320) SUBSCRIBE operation scheduled.
38 13988 [AWS-RemoteCtrl] [INFO ][MQTT][13988] (MQTT connection 20009320, SUBSCRIBE operation 2000a6e0) Waiting for operation completion.
39 14148 [AWS-RemoteCtrl] [INFO ][MQTT][14148] (MQTT connection 20009320, SUBSCRIBE operation 2000a6e0) Wait complete with result SUCCESS.
40 14148 [AWS-RemoteCtrl] [INFO ][MQTT][14148] (MQTT connection 20009320) SUBSCRIBE operation scheduled.
41 14148 [AWS-RemoteCtrl] [INFO ][MQTT][14148] (MQTT connection 20009320, SUBSCRIBE operation 2000a6e0) Waiting for operation completion.
42 14310 [AWS-RemoteCtrl] [INFO ][MQTT][14310] (MQTT connection 20009320, SUBSCRIBE operation 2000a6e0) Wait complete with result SUCCESS.
43 14323 [AWS-RemoteCtrl] [INFO ][MQTT][14323] (MQTT connection 20009320) MQTT PUBLISH operation queued.
44 14563 [iot_thread] [INFO ][Shadow][14563] Shadow UPDATE of aws_rpr_michal was ACCEPTED.
45 14572 [AWS-RemoteCtrl] AWS Remote Control Demo initialized.
46 14577 [AWS-RemoteCtrl] Use mobile application to control the remote device.
47 26245 [AWS-RemoteCtrl] Turn on LED Red
48 26250 [AWS-RemoteCtrl] [INFO ][MQTT][26249] (MQTT connection 20009320) MQTT PUBLISH operation queued.
49 26288 [iot_thread] [WARN ][Shadow][26288] Received a Shadow UPDATE response with no client token. This is possibly a response to a bad JSON document:
{"state":{"desired":{"LEDstate":1}},"metadata":{"desired":{"LEDstate":{"timestamp":1570799264}}},"vers50 26289 [iot_thread] [WARN ][Shadow][26288] Shadow UPDATE callback received an unknown operation.
51 26441 [iot_thread] [INFO ][Shadow][26441] Shadow UPDATE of aws_rpr_michal was ACCEPTED.
52 26449 [AWS-RemoteCtrl] Successfully performed update.
53 29042 [iot_thread] [WARN ][Shadow][29042] Shadow UPDATE callback received an unknown operation.
54 29052 [AWS-RemoteCtrl] Turn off LED Red
55 29057 [AWS-RemoteCtrl] [INFO ][MQTT][29056] (MQTT connection 20009320) MQTT PUBLISH operation queued.
56 29255 [iot_thread] [INFO ][Shadow][29255] Shadow UPDATE of aws_rpr_michal was ACCEPTED.
57 29263 [AWS-RemoteCtrl] Successfully performed update.
58 32304 [iot_thread] [WARN ][Shadow][32304] Shadow UPDATE callback received an unknown operation.
59 32313 [AWS-RemoteCtrl] Turn on LED Red
60 32317 [AWS-RemoteCtrl] [INFO ][MQTT][32317] (MQTT connection 20009320) MQTT PUBLISH operation queued.
61 32545 [iot_thread] [INFO ][Shadow][32545] Shadow UPDATE of aws_rpr_michal was ACCEPTED.
62 32553 [AWS-RemoteCtrl] Successfully performed update.
63 36180 [iot_thread] [WARN ][Shadow][36180] Shadow UPDATE callback received an unknown operation.
64 36189 [AWS-RemoteCtrl] Turn off LED Red
65 36194 [AWS-RemoteCtrl] [INFO ][MQTT][36194] (MQTT connection 20009320) MQTT PUBLISH operation queued.
66 36385 [iot_thread] [INFO ][Shadow][36385] Shadow UPDATE of aws_rpr_michal was ACCEPTED.
67 36393 [AWS-RemoteCtrl] Successfully performed update.
68 38163 [iot_thread] [WARN ][Shadow][38163] Shadow UPDATE callback received an unknown operation.
69 38173 [AWS-RemoteCtrl] Update accelerometer.
70 38180 [AWS-RemoteCtrl] [INFO ][MQTT][38180] (MQTT connection 20009320) MQTT PUBLISH operation queued.
71 38371 [iot_thread] [INFO ][Shadow][38371] Shadow UPDATE of aws_rpr_michal was ACCEPTED.
72 38379 [AWS-RemoteCtrl] Successfully performed update.
.
.
.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

