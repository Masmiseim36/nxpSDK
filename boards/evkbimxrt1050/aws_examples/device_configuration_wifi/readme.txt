Overview
========
This example demonstrates how the aws iot device can be configured by mobile application.


Prepare the Demo
================
Please update WiFi firmware to version 3.3.6 using "qca_fwupdate" demo.
Before running the demo it is need to configure AWS IoT Console and update some of project files:

1.  Create AWS Account: https://console.aws.amazon.com/console/home

2.  Open example's project and build it.

3.  Connect a USB cable between the PC host and the OpenSDA USB port on the target board.

4.  Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control

5.  Download the program to the target board.

6.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.


Prepare the Android application
The Android application requires Cognito service to authorize to AWS IoT in order to access device shadows. Use Amazon Cognito to create a new identity pool:

1.  In the Amazon Cognito Console https://console.aws.amazon.com/cognito/ select "Manage Federated Identities" and "Create new identity pool".

2.  Name your pool and ensure "Enable access to unauthenticated identities" is checked. This allows the sample application to assume the unauthenticated role associated with this identity pool.
    Note: to keep this example simple it makes use of unauthenticated users in the identity pool. This can be used for getting started and prototypes but unauthenticated users should typically only be given read-only permissions in production applications. More information on Cognito identity pools including the Cognito developer guide can be found here: http://aws.amazon.com/cognito/.

3.  To obtain the Pool ID constant, select "Edit identity pool" and copy Identity pool ID (it will look like <REGION>:<ID>). This Identity pool ID (<COGNITO POOL ID>) will be used in the application (policy and configuration file).

4.  To obtain Account ID, select account name in webpage menu bar and select "My account" from drop down menu. Make note of "Account ID" under "Account Settings".

5.  As part of creating the identity pool Cognito will setup two roles in Identity and Access Management (IAM) https://console.aws.amazon.com/iam/home#roles. These will be named something similar to: "Cognito_PoolNameAuth_Role" and "Cognito_PoolNameUnauth_Role".
    Create policy to be attached into "Cognito_PoolNameUnauth_Role" though "Policies" menu, selecting "Create policy", "Create Your Own Policy" and copying example policy below into "Policy Document" field and naming it for example "<THING NAME>Policy".

    {
        "Version": "2012-10-17",
        "Statement": [
            {
                "Effect": "Allow",
                "Action": [
                    "iot:ListThings",
                    "iot:ListPolicies",
                    "iot:CreateKeysAndCertificate",
                    "iot:AttachThingPrincipal",
                    "iot:CreateThing",
                    "iot:CreatePolicy",
                    "iot:AttachPolicy",
                    "iot:Connect",
                    "iot:Subscribe",
                    "iot:Publish",
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

7.  Prepare AwsDeviceConfigurationPreferences.properties file with yours AWS credentials. It's structure looks like this:

    customer_specific_endpoint=<ACCOUNT ID>.iot.<REGION>.amazonaws.com
    cognito_pool_id=<REGION>:<COGNITO POOL ID>
    region=<REGION>

    Then move file into your Android device.

8.  To run Android application do either:
    a) install and run pre-build apk on Android device (<SDK_Repository>\boards\<board_name>\aws_examples\remote_control\android\AwsRemoteControl.apk)
    b) open project in Android Studio, build it, attach Android device and Run application

    Application requires at least Android version 6.0 (Android SDK 23).

    Then in both cases when asked select AwsDeviceConfigurationPreferences.properties file with AWS IoT preferences.

    During device configuration you will need to input device login password (by default it is "admin") to begin communication with the device. This password can be changed.



Running the demo
================
The log below shows the output of the demo in the terminal window. The log can be different based on the actions, which you have done in the Android application.

The board tries to connect to configured WiFi network. If the connection is failed or these parameters of WiFi network are not set the board creates WiFi AP: aws_iot_ap.
Use the smart phone to connect to the WiFi AP and use the Android application to configure the parameters of WiFi network.

Board will create aws-iot-device on the network which can be discovered by Android application via mDNS service. User can select either of discovered boards and configure their AWS related settings (e.g. thing name, credentials, etc.) or other settings (e.g. mDNS host name).
After successful configuration board will connect to AWS IoT and AWS Shadow example will be run.


~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
.
.
.
5 509 [Tmr Svc] Starting WiFi...
6 2086 [Tmr Svc] WiFi module initialized.
7 5237 [Tmr Svc] WiFi connected to AP External-Internet.
8 5238 [Tmr Svc] IP Address acquired 192.168.0.245
9 5248 [MainDemoTask] [Shadow 0] MQTT: Creation of dedicated MQTT client succeeded.
10 5511 [MQTT] Looked up ah7mkbw0r336-ats.iot.us-west-2.amazonaws.com as 54.69.21.148
11 18642 [MainDemoTask] [Shadow 0] MQTT: Connect succeeded.
12 18963 [MainDemoTask] [Shadow 0] MQTT: Subscribe to accepted topic succeeded.
13 19283 [MainDemoTask] [Shadow 0] MQTT: Subscribe to rejected topic succeeded.
14 19297 [MainDemoTask] [Shadow 0] MQTT: Publish to operation topic succeeded.
15 19931 [MainDemoTask] [Shadow 0] MQTT: Unsubscribe from rejected topic succeeded.
16 20252 [MainDemoTask] [Shadow 0] MQTT: Subscribe to callback topic succeeded.
17 20255 [MainDemoTask] Shadow client initialized.
18 20260 [ShDemoUpdt] Performing Thing Shadow update.
19 20579 [ShDemoUpdt] [Shadow 0] MQTT: Subscribe to accepted topic succeeded.
20 20899 [ShDemoUpdt] [Shadow 0] MQTT: Subscribe to rejected topic succeeded.
21 20913 [ShDemoUpdt] [Shadow 0] MQTT: Publish to operation topic succeeded.
22 21231 [ShDemoUpdt] Successfully performed update.
23 21234 [Shd-IOT-0] Shd-IOT-0 changing desired state.
24 21237 [ShDemoUpdt] Performing Thing Shadow update.
25 21251 [ShDemoUpdt] [Shadow 0] MQTT: Publish to operation topic succeeded.
26 21669 [ShDemoUpdt] Successfully performed update.
27 21672 [Shd-IOT-0] Shd-IOT-0 done changing desired state.
28 26234 [Shd-IOT-0] Shd-IOT-0 changing desired state.
29 26237 [ShDemoUpdt] Performing Thing Shadow update.
30 26250 [ShDemoUpdt] [Shadow 0] MQTT: Publish to operation topic succeeded.
31 26589 [MQTT] Successfully added new reported state to update queue.
32 26592 [ShDemoUpdt] Successfully performed update.
34 26596 [ShDemoUpdt] Performing Thing Shadow update.
33 26595 [Shd-IOT-0] Shd-IOT-0 done changing desired state.
35 26611 [ShDemoUpdt] [Shadow 0] MQTT: Publish to operation topic succeeded.
36 26927 [ShDemoUpdt] Successfully performed update.
37 31234 [Shd-IOT-0] Shd-IOT-0 changing desired state.
38 31237 [ShDemoUpdt] Performing Thing Shadow update.
39 31250 [ShDemoUpdt] [Shadow 0] MQTT: Publish to operation topic succeeded.
40 31583 [MQTT] Successfully added new reported state to update queue.
41 31587 [ShDemoUpdt] Successfully performed update.
42 31589 [ShDemoUpdt] Performing Thing Shadow update.
43 31591 [Shd-IOT-0] Shd-IOT-0 done changing desired state.
44 31603 [ShDemoUpdt] [Shadow 0] MQTT: Publish to operation topic succeeded.
45 31931 [ShDemoUpdt] Successfully performed update.
46 36234 [Shd-IOT-0] Shd-IOT-0 changing desired state.
47 36237 [ShDemoUpdt] Performing Thing Shadow update.
48 36250 [ShDemoUpdt] [Shadow 0] MQTT: Publish to operation topic succeeded.
49 36584 [MQTT] Successfully added new reported state to update queue.
50 36588 [ShDemoUpdt] Successfully performed update.
51 36590 [ShDemoUpdt] Performing Thing Shadow update.
52 36592 [Shd-IOT-0] Shd-IOT-0 done changing desired state.
53 36604 [ShDemoUpdt] [Shadow 0] MQTT: Publish to operation topic succeeded.
54 36922 [ShDemoUpdt] Successfully performed update.
55 41234 [Shd-IOT-0] Shd-IOT-0 changing desired state.
56 41237 [ShDemoUpdt] Performing Thing Shadow update.
57 41250 [ShDemoUpdt] [Shadow 0] MQTT: Publish to operation topic succeeded.
58 41585 [MQTT] Successfully added new reported state to update queue.
59 41588 [ShDemoUpdt] Successfully performed update.
.
.
.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Hardware requirements
=====================
- Mini/micro USB cable
- EVKB-IMXRT1050 board
- GT202 Adaptor V1.04
- Personal Computer

Board settings
==============
Plug GT202 Adaptor board to FRDM stackable headers (J1, J2, J3, J4).
Remove the resistor R334, dis-connect J15, and weld 0Ω resistor to R278,R279,R280,R281.

Please use external power supply (J2), USB may not be sufficient.


Toolchain supported
===================
- IAR embedded Workbench  8.32.1
- Keil MDK  5.26
- GCC ARM Embedded  7.3.1
- MCUXpresso 10.3.0

