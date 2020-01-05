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
- Personal Computer
- Network cable RJ45 standard (Network with Internet access)

Board settings
==============
No special settings are required.
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
0 159 [Tmr Svc] Write certificate...
1 462 [Tmr Svc] Device credential provisioning succeeded.
2 2086 [Tmr Svc] Getting IP address from DHCP ...
3 5086 [Tmr Svc] IPv4 Address: 192.168.2.102
4 5086 [Tmr Svc] DHCP OK
5 5092 [AWS-RemoteCtrl] [INFO ][INIT][5092] SDK successfully initialized.
6 5099 [AWS-RemoteCtrl] [INFO ][Shadow][5099] Shadow library successfully initialized.
7 10082 [AWS-RemoteCtrl] [INFO ][MQTT][10082] Establishing new MQTT connection.
8 10091 [AWS-RemoteCtrl] [INFO ][MQTT][10091] Anonymous metrics (SDK language, SDK version) will be provided to AWS IoT. Recompile with AWS_IOT_MQTT_ENABLE_METRICS set to 0 to disable.
9 10108 [AWS-RemoteCtrl] [INFO ][MQTT][10108] (MQTT connection 2000ad58, CONNECT operation 2000af30) Waiting for operation completion.
10 10303 [AWS-RemoteCtrl] [INFO ][MQTT][10303] (MQTT connection 2000ad58, CONNECT operation 2000af30) Wait complete with result SUCCESS.
11 10315 [AWS-RemoteCtrl] [INFO ][MQTT][10315] New MQTT connection 2000a450 established.
12 10323 [AWS-RemoteCtrl] [INFO ][MQTT][10323] (MQTT connection 2000ad58) SUBSCRIBE operation scheduled.
13 10324 [AWS-RemoteCtrl] [INFO ][MQTT][10324] (MQTT connection 2000ad58, SUBSCRIBE operation 2000bec0) Waiting for operation completion.
14 10483 [AWS-RemoteCtrl] [INFO ][MQTT][10483] (MQTT connection 2000ad58, SUBSCRIBE operation 2000bec0) Wait complete with result SUCCESS.
15 10483 [AWS-RemoteCtrl] [INFO ][MQTT][10483] (MQTT connection 2000ad58) SUBSCRIBE operation scheduled.
16 10484 [AWS-RemoteCtrl] [INFO ][MQTT][10484] (MQTT connection 2000ad58, SUBSCRIBE operation 2000bec0) Waiting for operation completion.
17 10646 [AWS-RemoteCtrl] [INFO ][MQTT][10646] (MQTT connection 2000ad58, SUBSCRIBE operation 2000bec0) Wait complete with result SUCCESS.
18 10659 [AWS-RemoteCtrl] [INFO ][MQTT][10659] (MQTT connection 2000ad58) MQTT PUBLISH operation queued.
19 10844 [iot_thread] [INFO ][Shadow][10844] Shadow DELETE of aws_rpr_michal was ACCEPTED.
20 10852 [AWS-RemoteCtrl] [INFO ][MQTT][10852] (MQTT connection 2000ad58) UNSUBSCRIBE operation scheduled.
21 10852 [AWS-RemoteCtrl] [INFO ][MQTT][10852] (MQTT connection 2000ad58, UNSUBSCRIBE operation 2000bec0) Waiting for operation completion.
22 11193 [AWS-RemoteCtrl] [INFO ][MQTT][11193] (MQTT connection 2000ad58, UNSUBSCRIBE operation 2000bec0) Wait complete with result SUCCESS.
23 11194 [AWS-RemoteCtrl] [INFO ][MQTT][11194] (MQTT connection 2000ad58) UNSUBSCRIBE operation scheduled.
24 11194 [AWS-RemoteCtrl] [INFO ][MQTT][11194] (MQTT connection 2000ad58, UNSUBSCRIBE operation 2000bec0) Waiting for operation completion.
25 11352 [AWS-RemoteCtrl] [INFO ][MQTT][11352] (MQTT connection 2000ad58, UNSUBSCRIBE operation 2000bec0) Wait complete with result SUCCESS.
26 11364 [AWS-RemoteCtrl] [INFO ][Shadow][11364] (aws_rpr_michal) Modifying Shadow DELTA callback.
27 11373 [AWS-RemoteCtrl] [INFO ][Shadow][11373] (aws_rpr_michal) Adding new DELTA callback.
28 11382 [AWS-RemoteCtrl] [INFO ][MQTT][11382] (MQTT connection 2000ad58) SUBSCRIBE operation scheduled.
29 11382 [AWS-RemoteCtrl] [INFO ][MQTT][11382] (MQTT connection 2000ad58, SUBSCRIBE operation 2000bec0) Waiting for operation completion.
30 11551 [AWS-RemoteCtrl] [INFO ][MQTT][11551] (MQTT connection 2000ad58, SUBSCRIBE operation 2000bec0) Wait complete with result SUCCESS.
31 11563 [AWS-RemoteCtrl] [INFO ][Shadow][11563] (aws_rpr_michal) Shadow DELTA callback operation complete with result SUCCESS.
32 11575 [AWS-RemoteCtrl] [INFO ][Shadow][11575] (aws_rpr_michal) Modifying Shadow UPDATED callback.
33 11584 [AWS-RemoteCtrl] [INFO ][Shadow][11584] (aws_rpr_michal) Shadow UPDATED callback operation complete with result SUCCESS.
34 11596 [AWS-RemoteCtrl] [INFO ][MQTT][11596] (MQTT connection 2000ad58) SUBSCRIBE operation scheduled.
35 11596 [AWS-RemoteCtrl] [INFO ][MQTT][11596] (MQTT connection 2000ad58, SUBSCRIBE operation 2000c128) Waiting for operation completion.
36 11757 [AWS-RemoteCtrl] [INFO ][MQTT][11757] (MQTT connection 2000ad58, SUBSCRIBE operation 2000c128) Wait complete with result SUCCESS.
37 11757 [AWS-RemoteCtrl] [INFO ][MQTT][11757] (MQTT connection 2000ad58) SUBSCRIBE operation scheduled.
38 11757 [AWS-RemoteCtrl] [INFO ][MQTT][11757] (MQTT connection 2000ad58, SUBSCRIBE operation 2000c128) Waiting for operation completion.
39 11975 [AWS-RemoteCtrl] [INFO ][MQTT][11975] (MQTT connection 2000ad58, SUBSCRIBE operation 2000c128) Wait complete with result SUCCESS.
40 11988 [AWS-RemoteCtrl] [INFO ][MQTT][11988] (MQTT connection 2000ad58) MQTT PUBLISH operation queued.
41 12175 [iot_thread] [INFO ][Shadow][12175] Shadow UPDATE of aws_rpr_michal was ACCEPTED.
42 12183 [AWS-RemoteCtrl] AWS Remote Control Demo initialized.
43 12189 [AWS-RemoteCtrl] Use mobile application to control the remote device.
44 27034 [iot_thread] [WARN ][Shadow][27034] Shadow UPDATE callback received an unknown operation.
45 27044 [AWS-RemoteCtrl] Turn on LED Red
46 27048 [AWS-RemoteCtrl] [INFO ][MQTT][27048] (MQTT connection 2000ad58) MQTT PUBLISH operation queued.
47 27366 [iot_thread] [INFO ][Shadow][27366] Shadow UPDATE of aws_rpr_michal was ACCEPTED.
48 27375 [AWS-RemoteCtrl] Successfully performed update.
49 30311 [iot_thread] [WARN ][Shadow][30311] Shadow UPDATE callback received an unknown operation.
50 30320 [AWS-RemoteCtrl] Turn off LED Red
51 30324 [AWS-RemoteCtrl] [INFO ][MQTT][30324] (MQTT connection 2000ad58) MQTT PUBLISH operation queued.
52 30641 [iot_thread] [INFO ][Shadow][30641] Shadow UPDATE of aws_rpr_michal was ACCEPTED.
53 30650 [AWS-RemoteCtrl] Successfully performed update.
54 32474 [AWS-RemoteCtrl] Update accelerometer.
.
.
.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

