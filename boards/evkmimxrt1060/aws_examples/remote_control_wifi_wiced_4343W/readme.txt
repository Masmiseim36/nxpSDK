Overview
========
This example demonstrates how the board can be controlled by Android mobile application with usage the AWS IoT.
User LEDs on the board can be controlled and also an action can be invoked to read data from accelerometer and report it to AWS IoT device shadow.
Please take look into section "Board settings" if there are some board limitations to control LEDs or read data from accelerometer.


Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- GCC ARM Embedded  8.3.1
- MCUXpresso  11.1.0

Hardware requirements
=====================
- Micro USB cable
- evkmimxrt1060 board
- Personal Computer
- Murata 1DX M.2 module
- Murata uSD M.2 Adapter
- 1 plug to receptable header cable

Board settings
==============
The communication with accelerometer sensor is disabled by default, because some boards have not accelerometer sensor U32.
If the board has accelerometer sensor U32, the using of it can be enabled by definition of project macro: BOARD_ACCEL_FXOS.

The following pins between the evkmimxrt1060 board and Murata uSD M.2 Adapter are connected using the plug to receptable cables:
- evkmimxrt1060 board's connector J22, pin 3 to Murata uSD M.2 Adapter's connector J9, pin 3

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
0 114 [Tmr Svc] Write certificate...
1 239 [Tmr Svc] Device credential provisioning succeeded.
2 240 [Tmr Svc] Starting WiFi...

 AsyncInterrupt is not supported 
WLAN MAC Address : A0:C9:A0:3D:EE:8F
WLAN Firmware    : wl0: Feb 12 2018 04:08:14 version 7.79.2 (r683798 CY) FWID 01-27b63357
WLAN CLM         : API: 12.2 Data: 9.10.39 Compiler: 1.29.4 ClmImport: 1.36.3 Creation: 2018-02-12 04:00:50 
3 1177 [Tmr Svc] WiFi module initialized.
4 4942 [Tmr Svc] WiFi connected to AP Guest.
5 4942 [Tmr Svc] IP Address acquired 192.168.2.100
6 4951 [AWS-RemoteCtrl] [INFO ][INIT][4950] SDK successfully initialized.
7 4957 [AWS-RemoteCtrl] [INFO ][Shadow][4957] Shadow library successfully initialized.
8 8125 [AWS-RemoteCtrl] [INFO ][MQTT][8125] Establishing new MQTT connection.
9 8135 [AWS-RemoteCtrl] [INFO ][MQTT][8134] Anonymous metrics (SDK language, SDK version) will be provided to AWS IoT. Recompile with AWS_IOT_MQTT_ENABLE_METRICS set to 0 to disable.
10 8151 [AWS-RemoteCtrl] [INFO ][MQTT][8151] (MQTT connection 20209568, CONNECT operation 20209740) Waiting for operation completion.
11 8345 [AWS-RemoteCtrl] [INFO ][MQTT][8344] (MQTT connection 20209568, CONNECT operation 20209740) Wait complete with result SUCCESS.
12 8357 [AWS-RemoteCtrl] [INFO ][MQTT][8357] New MQTT connection 20208a68 established.
13 8365 [AWS-RemoteCtrl] [INFO ][MQTT][8365] (MQTT connection 20209568) SUBSCRIBE operation scheduled.
14 8365 [AWS-RemoteCtrl] [INFO ][MQTT][8365] (MQTT connection 20209568, SUBSCRIBE operation 2020a6c8) Waiting for operation completion.
15 8522 [AWS-RemoteCtrl] [INFO ][MQTT][8522] (MQTT connection 20209568, SUBSCRIBE operation 2020a6c8) Wait complete with result SUCCESS.
16 8522 [AWS-RemoteCtrl] [INFO ][MQTT][8522] (MQTT connection 20209568) SUBSCRIBE operation scheduled.
17 8523 [AWS-RemoteCtrl] [INFO ][MQTT][8522] (MQTT connection 20209568, SUBSCRIBE operation 2020a6c8) Waiting for operation completion.
18 8684 [AWS-RemoteCtrl] [INFO ][MQTT][8684] (MQTT connection 20209568, SUBSCRIBE operation 2020a6c8) Wait complete with result SUCCESS.
19 8697 [AWS-RemoteCtrl] [INFO ][MQTT][8697] (MQTT connection 20209568) MQTT PUBLISH operation queued.
20 8882 [iot_thread] [WARN ][Shadow][8882] Shadow DELETE of aws_rpr_michal was REJECTED.
21 8882 [iot_thread] [WARN ][Shadow][8882] Code 404: "No shadow exists with name: 'aws_rpr_michal'".
22 8899 [AWS-RemoteCtrl] [INFO ][MQTT][8899] (MQTT connection 20209568) UNSUBSCRIBE operation scheduled.
23 8900 [AWS-RemoteCtrl] [INFO ][MQTT][8899] (MQTT connection 20209568, UNSUBSCRIBE operation 2020a6c8) Waiting for operation completion.
24 9178 [AWS-RemoteCtrl] [INFO ][MQTT][9178] (MQTT connection 20209568, UNSUBSCRIBE operation 2020a6c8) Wait complete with result SUCCESS.
25 9178 [AWS-RemoteCtrl] [INFO ][MQTT][9178] (MQTT connection 20209568) UNSUBSCRIBE operation scheduled.
26 9179 [AWS-RemoteCtrl] [INFO ][MQTT][9178] (MQTT connection 20209568, UNSUBSCRIBE operation 2020a6c8) Waiting for operation completion.
27 9338 [AWS-RemoteCtrl] [INFO ][MQTT][9338] (MQTT connection 20209568, UNSUBSCRIBE operation 2020a6c8) Wait complete with result SUCCESS.
28 9351 [AWS-RemoteCtrl] [INFO ][Shadow][9351] (aws_rpr_michal) Modifying Shadow DELTA callback.
29 9359 [AWS-RemoteCtrl] [INFO ][Shadow][9359] (aws_rpr_michal) Adding new DELTA callback.
30 9368 [AWS-RemoteCtrl] [INFO ][MQTT][9368] (MQTT connection 20209568) SUBSCRIBE operation scheduled.
31 9368 [AWS-RemoteCtrl] [INFO ][MQTT][9368] (MQTT connection 20209568, SUBSCRIBE operation 2020a680) Waiting for operation completion.
32 9525 [AWS-RemoteCtrl] [INFO ][MQTT][9525] (MQTT connection 20209568, SUBSCRIBE operation 2020a680) Wait complete with result SUCCESS.
33 9537 [AWS-RemoteCtrl] [INFO ][Shadow][9537] (aws_rpr_michal) Shadow DELTA callback operation complete with result SUCCESS.
34 9549 [AWS-RemoteCtrl] [INFO ][Shadow][9549] (aws_rpr_michal) Modifying Shadow UPDATED callback.
35 9557 [AWS-RemoteCtrl] [INFO ][Shadow][9557] (aws_rpr_michal) Shadow UPDATED callback operation complete with result SUCCESS.
36 9569 [AWS-RemoteCtrl] [INFO ][MQTT][9569] (MQTT connection 20209568) SUBSCRIBE operation scheduled.
37 9569 [AWS-RemoteCtrl] [INFO ][MQTT][9569] (MQTT connection 20209568, SUBSCRIBE operation 2020aa00) Waiting for operation completion.
38 9731 [AWS-RemoteCtrl] [INFO ][MQTT][9731] (MQTT connection 20209568, SUBSCRIBE operation 2020aa00) Wait complete with result SUCCESS.
39 9731 [AWS-RemoteCtrl] [INFO ][MQTT][9731] (MQTT connection 20209568) SUBSCRIBE operation scheduled.
40 9732 [AWS-RemoteCtrl] [INFO ][MQTT][9732] (MQTT connection 20209568, SUBSCRIBE operation 2020aa00) Waiting for operation completion.
41 9888 [AWS-RemoteCtrl] [INFO ][MQTT][9888] (MQTT connection 20209568, SUBSCRIBE operation 2020aa00) Wait complete with result SUCCESS.
42 9901 [AWS-RemoteCtrl] [INFO ][MQTT][9901] (MQTT connection 20209568) MQTT PUBLISH operation queued.
43 10091 [iot_thread] [INFO ][Shadow][10091] Shadow UPDATE of aws_rpr_michal was ACCEPTED.
44 10099 [AWS-RemoteCtrl] AWS Remote Control Demo initialized.
45 10105 [AWS-RemoteCtrl] Use mobile application to control the remote device.
46 26970 [iot_thread] [WARN ][Shadow][26970] Shadow UPDATE callback received an unknown operation.
47 26979 [AWS-RemoteCtrl] Turn on LED
48 26983 [AWS-RemoteCtrl] [INFO ][MQTT][26983] (MQTT connection 20209568) MQTT PUBLISH operation queued.
49 27294 [iot_thread] [INFO ][Shadow][27294] Shadow UPDATE of aws_rpr_michal was ACCEPTED.
50 27302 [AWS-RemoteCtrl] Successfully performed update.
51 34735 [iot_thread] [WARN ][Shadow][34734] Shadow UPDATE callback received an unknown operation.
52 34744 [AWS-RemoteCtrl] Turn off LED
53 34749 [AWS-RemoteCtrl] [INFO ][MQTT][34748] (MQTT connection 20209568) MQTT PUBLISH operation queued.
54 35059 [iot_thread] [INFO ][Shadow][35059] Shadow UPDATE of aws_rpr_michal was ACCEPTED.
55 35067 [AWS-RemoteCtrl] Successfully performed update.
56 36795 [iot_thread] [WARN ][Shadow][36795] Shadow UPDATE callback received an unknown operation.
57 36805 [AWS-RemoteCtrl] Update accelerometer.
58 36812 [AWS-RemoteCtrl] [INFO ][MQTT][36812] (MQTT connection 20209568) MQTT PUBLISH operation queued.
59 37121 [iot_thread] [INFO ][Shadow][37121] Shadow UPDATE of aws_rpr_michal was ACCEPTED.
60 37129 [AWS-RemoteCtrl] Successfully performed update.
.
.
.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

