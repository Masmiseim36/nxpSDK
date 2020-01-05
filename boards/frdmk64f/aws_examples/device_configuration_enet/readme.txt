Overview
========
This example demonstrates how the aws iot device can be configured by mobile application.



Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- Keil MDK  5.29
- GCC ARM Embedded  8.3.1
- MCUXpresso  11.1.0

Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-K64F board
- Personal Computer
- Network cable RJ45 standard (Network with Internet access)

Board settings
==============
No special settings are required.
Prepare the Demo
================
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

5.  Connect the board's RJ45 to network with Internet access (IP address to the board is assigned by the DHCP server). Make sure the connection on port 8883 is not blocked.

6.  Download the program to the target board.

7.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.


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
    a) install and run pre-build apk on Android device (<SDK_Repository>\boards\<board_name>\aws_examples\device_configuration_android\AwsDeviceConfiguration.apk)
    b) open project in Android Studio, build it, attach Android device and Run application

    Application requires at least Android version 6.0 (Android SDK 23).

    Then in both cases when asked select AwsDeviceConfigurationPreferences.properties file with AWS IoT preferences.

    During device configuration you will need to input device login password (by default it is "admin") to begin communication with the device. This password can be changed.



Running the demo
================
The log below shows the output of the demo in the terminal window. The log can be different based on the actions, which you have done in the Android application.

Board will create aws-iot-device on the network which can be discovered by Android application via mDNS service. User can select either of discovered boards and configure their AWS related settings (e.g. thing name, credentials, etc.) or other settings (e.g. mDNS host name).
After successful configuration board will connect to AWS IoT and AWS Shadow example will be run.


~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Initializing PHY...
.
.
.
0 1657 [Tmr Svc] Getting IP address from DHCP ...
1 4657 [Tmr Svc] IPv4 Address: 192.168.2.101
2 4657 [Tmr Svc] DHCP OK
3 4794 [MainDemoTask] [INFO ][INIT][4794] SDK successfully initialized.
4 4800 [MainDemoTask] [INFO ][Shadow][4800] Shadow library successfully initialized.
5 9874 [MainDemoTask] [INFO ][MQTT][9874] Establishing new MQTT connection.
6 9881 [MainDemoTask] [INFO ][MQTT][9881] Anonymous metrics (SDK language, SDK version) will be provided to AWS IoT. Recompile with AWS_IOT_MQTT_ENABLE_METRICS set to 0 to disable.
7 9898 [MainDemoTask] [INFO ][MQTT][9898] (MQTT connection 20007218, CONNECT operation 2000b9d0) Waiting for operation completion.
8 10116 [MainDemoTask] [INFO ][MQTT][10116] (MQTT connection 20007218, CONNECT operation 2000b9d0) Wait complete with result SUCCESS.
9 10128 [MainDemoTask] [INFO ][MQTT][10128] New MQTT connection 2000b060 established.
10 10136 [MainDemoTask] [INFO ][MQTT][10136] (MQTT connection 20007218) SUBSCRIBE operation scheduled.
11 10136 [MainDemoTask] [INFO ][MQTT][10136] (MQTT connection 20007218, SUBSCRIBE operation 2000bcb8) Waiting for operation completion.
12 10295 [MainDemoTask] [INFO ][MQTT][10295] (MQTT connection 20007218, SUBSCRIBE operation 2000bcb8) Wait complete with result SUCCESS.
13 10295 [MainDemoTask] [INFO ][MQTT][10295] (MQTT connection 20007218) SUBSCRIBE operation scheduled.
14 10295 [MainDemoTask] [INFO ][MQTT][10295] (MQTT connection 20007218, SUBSCRIBE operation 2000bad8) Waiting for operation completion.
15 10457 [MainDemoTask] [INFO ][MQTT][10457] (MQTT connection 20007218, SUBSCRIBE operation 2000bad8) Wait complete with result SUCCESS.
16 10470 [MainDemoTask] [INFO ][MQTT][10470] (MQTT connection 20007218) MQTT PUBLISH operation queued.
17 10657 [iot_thread] [INFO ][Shadow][10657] Shadow DELETE of test-michal-rpr was ACCEPTED.
18 10665 [MainDemoTask] [INFO ][MQTT][10665] (MQTT connection 20007218) UNSUBSCRIBE operation scheduled.
19 10665 [MainDemoTask] [INFO ][MQTT][10665] (MQTT connection 20007218, UNSUBSCRIBE operation 2000bad8) Waiting for operation completion.
20 10958 [MainDemoTask] [INFO ][MQTT][10958] (MQTT connection 20007218, UNSUBSCRIBE operation 2000bad8) Wait complete with result SUCCESS.
21 10958 [MainDemoTask] [INFO ][MQTT][10958] (MQTT connection 20007218) UNSUBSCRIBE operation scheduled.
22 10958 [MainDemoTask] [INFO ][MQTT][10958] (MQTT connection 20007218, UNSUBSCRIBE operation 2000bad8) Waiting for operation completion.
23 11118 [MainDemoTask] [INFO ][MQTT][11117] (MQTT connection 20007218, UNSUBSCRIBE operation 2000bad8) Wait complete with result SUCCESS.
24 11130 [MainDemoTask] [INFO ][Shadow][11130] (test-michal-rpr) Modifying Shadow DELTA callback.
25 11139 [MainDemoTask] [INFO ][Shadow][11139] (test-michal-rpr) Adding new DELTA callback.
26 11147 [MainDemoTask] [INFO ][MQTT][11147] (MQTT connection 20007218) SUBSCRIBE operation scheduled.
27 11147 [MainDemoTask] [INFO ][MQTT][11147] (MQTT connection 20007218, SUBSCRIBE operation 2000ba58) Waiting for operation completion.
28 11310 [MainDemoTask] [INFO ][MQTT][11310] (MQTT connection 20007218, SUBSCRIBE operation 2000ba58) Wait complete with result SUCCESS.
29 11322 [MainDemoTask] [INFO ][Shadow][11322] (test-michal-rpr) Shadow DELTA callback operation complete with result SUCCESS.
30 11334 [MainDemoTask] [INFO ][Shadow][11333] (test-michal-rpr) Modifying Shadow UPDATED callback.
31 11343 [MainDemoTask] [INFO ][Shadow][11342] (test-michal-rpr) Shadow UPDATED callback operation complete with result SUCCESS.
32 11354 [MainDemoTask] Shadow client initialized.
33 11359 [ShDemoUpdt] Performing Thing Shadow update.
34 11359 [ShDemoUpdt] [INFO ][MQTT][11359] (MQTT connection 20007218) SUBSCRIBE operation scheduled.
35 11359 [ShDemoUpdt] [INFO ][MQTT][11359] (MQTT connection 20007218, SUBSCRIBE operation 2000c9b0) Waiting for operation completion.
36 11517 [ShDemoUpdt] [INFO ][MQTT][11517] (MQTT connection 20007218, SUBSCRIBE operation 2000c9b0) Wait complete with result SUCCESS.
37 11517 [ShDemoUpdt] [INFO ][MQTT][11517] (MQTT connection 20007218) SUBSCRIBE operation scheduled.
38 11517 [ShDemoUpdt] [INFO ][MQTT][11517] (MQTT connection 20007218, SUBSCRIBE operation 200087b8) Waiting for operation completion.
39 11673 [ShDemoUpdt] [INFO ][MQTT][11673] (MQTT connection 20007218, SUBSCRIBE operation 200087b8) Wait complete with result SUCCESS.
40 11675 [ShDemoUpdt] [INFO ][MQTT][11675] (MQTT connection 20007218) MQTT PUBLISH operation queued.
41 11866 [iot_thread] [INFO ][Shadow][11866] Shadow UPDATE of test-michal-rpr was ACCEPTED.
42 11867 [ShDemoUpdt] Successfully performed update.
43 11867 [ShDemoUpdt] Performing Thing Shadow update.
44 11868 [ShDemoUpdt] [INFO ][MQTT][11868] (MQTT connection 20007218) MQTT PUBLISH operation queued.
45 11868 [Shd-IOT-0] Shd-IOT-0 changing desired state.
46 12201 [iot_thread] [INFO ][Shadow][12201] Shadow UPDATE of test-michal-rpr was ACCEPTED.
47 12202 [ShDemoUpdt] Successfully performed update.
48 12202 [ShDemoUpdt] Performing Thing Shadow update.
49 12203 [ShDemoUpdt] [INFO ][MQTT][12203] (MQTT connection 20007218) MQTT PUBLISH operation queued.
50 12203 [Shd-IOT-1] Shd-IOT-1 changing desired state.
51 12542 [iot_thread] [INFO ][Shadow][12542] Shadow UPDATE of test-michal-rpr was ACCEPTED.
52 12543 [ShDemoUpdt] Successfully performed update.
53 12543 [ShDemoUpdt] Performing Thing Shadow update.
54 12544 [ShDemoUpdt] [INFO ][MQTT][12544] (MQTT connection 20007218) MQTT PUBLISH operation queued.
55 12544 [Shd-IOT-0] Shd-IOT-0 done changing desired state.
56 12892 [iot_thread] [INFO ][Shadow][12892] Shadow UPDATE of test-michal-rpr was ACCEPTED.
57 12893 [ShDemoUpdt] Successfully performed update.
.
.
.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

