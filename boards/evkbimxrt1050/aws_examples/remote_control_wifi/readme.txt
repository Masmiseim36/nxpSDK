Overview
========
This example demonstrates how the board can be controlled by Android mobile application with usage the AWS IoT.
User LEDs on the board can be controlled and also an action can be invoked to read data from accelerometer and report it to AWS IoT device shadow.



Toolchain supported
===================
- Keil MDK 5.24a
- IAR embedded Workbench 8.22.2
- GCC ARM Embedded 7-2017-q4-major
- MCUXpresso10.2.0

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

Prepare the Demo
================
Before running the demo it is need to configure AWS IoT Console and update some of project files:

1.  Create AWS Account: https://console.aws.amazon.com/console/home

2.  Configure device in the AWS IoT Console base on this guide: https://docs.aws.amazon.com/iot/latest/developerguide/iot-sdk-setup.html

    Make note of example's "Thing name" and "REST API endpoint". These strings need to be set in the "aws_clientcredential.h".

    Example:
        static const char clientcredentialMQTT_BROKER_ENDPOINT[] = "abcdefgh123456.iot.us-west-2.amazonaws.com";
        #define clientcredentialIOT_THING_NAME "MyExample"

    In the next step you will get the "device certificate" and the "primary key". Each of the certificates needs to be opened in text editor and its content copied into the "aws_clientcredential_keys.h".
    Or you can use the CertificateConfigurator.html (mcu-sdk-2.0\rtos\amazon-freertos\demos\common\devmode_key_provisioning\CertificateConfigurationTool) to generate the "aws_clientcredential_keys.h".

    Example:
        static const char clientcredentialCLIENT_CERTIFICATE_PEM[] = "Paste client certificate here.";

        Needs to be changed to:

        static const char clientcredentialCLIENT_CERTIFICATE_PEM[] =
            "-----BEGIN CERTIFICATE-----\n"
            "MIIDWTCCAkGgAwIBAgIUPwbiJBIJhO6eF498l1GZ8siO/K0wDQYJKoZIhvcNAQEL\n"
            .
            .
            "KByzyTutxTeI9UKcIPFxK40s4qF50a40/6UFxrGueW+TzZ4iubWzP7eG+47r\n"
            "-----END CERTIFICATE-----\n";

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
0 0 [Tmr Svc] Starting key provisioning...
1 0 [Tmr Svc] Write root certificate...
2 0 [Tmr Svc] Write device private key...
3 6 [Tmr Svc] Write device certificate...
4 13 [Tmr Svc] Key provisioning done...
5 14 [Tmr Svc] Starting WiFi...
6 1264 [Tmr Svc] WiFi module initialized.
7 6281 [Tmr Svc] WiFi connected to AP RedmiAWS.
8 9284 [Tmr Svc] IP Address acquired 192.168.43.251
9 9289 [AWS-RemoteCtrl] [Shadow 0] MQTT: Creation of dedicated MQTT client succeeded.
10 9297 [AWS-RemoteCtrl] Sending command to MQTT task.
11 9302 [MQTT] Received message 10000 from queue.
12 13738 [MQTT] Looked up a254jqzk0kcvf0.iot.us-west-2.amazonaws.com as 35.165.74.145
13 30852 [MQTT] MQTT Connect was accepted. Connection established.
14 30852 [MQTT] Notifying task.
15 30861 [AWS-RemoteCtrl] Command sent to MQTT task passed.
16 30866 [AWS-RemoteCtrl] [Shadow 0] MQTT: Connect succeeded.
17 30872 [AWS-RemoteCtrl] Sending command to MQTT task.
18 30877 [MQTT] Received message 20000 from queue.
19 31486 [MQTT] MQTT Subscribe was accepted. Subscribed.
20 31486 [MQTT] Notifying task.
21 31494 [AWS-RemoteCtrl] Command sent to MQTT task passed.
22 31499 [AWS-RemoteCtrl] [Shadow 0] MQTT: Subscribe to accepted topic succeeded.
23 31507 [AWS-RemoteCtrl] Sending command to MQTT task.
24 31512 [MQTT] Received message 30000 from queue.
25 32222 [MQTT] MQTT Subscribe was accepted. Subscribed.
26 32222 [MQTT] Notifying task.
27 32230 [AWS-RemoteCtrl] Command sent to MQTT task passed.
28 32235 [AWS-RemoteCtrl] [Shadow 0] MQTT: Subscribe to rejected topic succeeded.
29 32243 [AWS-RemoteCtrl] Sending command to MQTT task.
30 32248 [MQTT] Received message 40000 from queue.
31 32250 [MQTT] Notifying task.
32 32257 [AWS-RemoteCtrl] Command sent to MQTT task passed.
33 32263 [AWS-RemoteCtrl] [Shadow 0] MQTT: Publish to operation topic succeeded.
34 33060 [AWS-RemoteCtrl] Sending command to MQTT task.
35 33065 [MQTT] Received message 50000 from queue.
36 33775 [MQTT] MQTT Unsubscribe was successful.
37 33775 [MQTT] Notifying task.
38 33782 [AWS-RemoteCtrl] Command sent to MQTT task passed.
39 33788 [AWS-RemoteCtrl] [Shadow 0] MQTT: Unsubscribe from rejected topic succeeded.
40 33796 [AWS-RemoteCtrl] Sending command to MQTT task.
41 33801 [MQTT] Received message 60000 from queue.
42 34511 [MQTT] MQTT Subscribe was accepted. Subscribed.
43 34511 [MQTT] Notifying task.
44 34519 [AWS-RemoteCtrl] Command sent to MQTT task passed.
45 34524 [AWS-RemoteCtrl] [Shadow 0] MQTT: Subscribe to callback topic succeeded.
46 34532 [AWS-RemoteCtrl] Sending command to MQTT task.
47 34537 [MQTT] Received message 70000 from queue.
48 35247 [MQTT] MQTT Subscribe was accepted. Subscribed.
49 35247 [MQTT] Notifying task.
50 35255 [AWS-RemoteCtrl] Command sent to MQTT task passed.
51 35260 [AWS-RemoteCtrl] [Shadow 0] MQTT: Subscribe to accepted topic succeeded.
52 35268 [AWS-RemoteCtrl] Sending command to MQTT task.
53 35273 [MQTT] Received message 80000 from queue.
54 35882 [MQTT] MQTT Subscribe was accepted. Subscribed.
55 35882 [MQTT] Notifying task.
56 35890 [AWS-RemoteCtrl] Command sent to MQTT task passed.
57 35895 [AWS-RemoteCtrl] [Shadow 0] MQTT: Subscribe to rejected topic succeeded.
58 35903 [AWS-RemoteCtrl] Sending command to MQTT task.
59 35908 [MQTT] Received message 90000 from queue.
60 35911 [MQTT] Notifying task.
61 35918 [AWS-RemoteCtrl] Command sent to MQTT task passed.
62 35923 [AWS-RemoteCtrl] [Shadow 0] MQTT: Publish to operation topic succeeded.
63 37432 [AWS-RemoteCtrl] AWS Remote Control Demo initialized.
64 37438 [AWS-RemoteCtrl] Use mobile application to control the remote device.
65 57537 [AWS-RemoteCtrl] Turn on LED1
66 57540 [AWS-RemoteCtrl] Sending command to MQTT task.
67 57546 [MQTT] Received message a0000 from queue.
68 57548 [MQTT] Notifying task.
69 57555 [AWS-RemoteCtrl] Command sent to MQTT task passed.
70 57560 [AWS-RemoteCtrl] [Shadow 0] MQTT: Publish to operation topic succeeded.
71 57854 [AWS-RemoteCtrl] Successfully performed update.
72 57860 [AWS-RemoteCtrl] [Shadow 0] MQTT: Return MQTT buffer succeeded.
73 69273 [AWS-RemoteCtrl] Turn off LED1
74 69277 [AWS-RemoteCtrl] Sending command to MQTT task.
75 69282 [MQTT] Received message b0000 from queue.
76 69284 [MQTT] Notifying task.
77 69291 [AWS-RemoteCtrl] Command sent to MQTT task passed.
78 69297 [AWS-RemoteCtrl] [Shadow 0] MQTT: Publish to operation topic succeeded.
79 70297 [AWS-RemoteCtrl] Successfully performed update.
80 70303 [AWS-RemoteCtrl] [Shadow 0] MQTT: Return MQTT buffer succeeded.
81 72020 [AWS-RemoteCtrl] Update accelerometer.
82 72028 [AWS-RemoteCtrl] Sending command to MQTT task.
83 72033 [MQTT] Received message c0000 from queue.
84 72035 [MQTT] Notifying task.
85 72042 [AWS-RemoteCtrl] Command sent to MQTT task passed.
86 72048 [AWS-RemoteCtrl] [Shadow 0] MQTT: Publish to operation topic succeeded.
87 73757 [AWS-RemoteCtrl] Successfully performed update.
88 73762 [AWS-RemoteCtrl] [Shadow 0] MQTT: Return MQTT buffer succeeded.
.
.
.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

