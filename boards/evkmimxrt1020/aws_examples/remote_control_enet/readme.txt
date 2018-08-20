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
- Micro USB cable
- EVK-MIMXRT1020 board
- Personal Computer
- Network cable RJ45 standard (Network with Internet access)

Board settings
==============
No special settings are required.

Note:
If sensor chip(U26) is not welded on the board.
     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
       Sensor device initialize failed!
       Please check the sensor chip.
     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


Note:
To debug in qspiflash, following steps are needed:
1. Select the flash target and compile.
3. Set the SW8: 1 off 2 off 3 on 4 off, then power on the board and connect USB cable to J23.
4. Start debugging in IDE.
   - Keil: Click "Download (F8)" to program the image to qspiflash first then clicking "Start/Stop Debug Session (Ctrl+F5)" to start debugging.
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

3.  Open example's project and build it.

4.  Connect a USB cable between the PC host and the OpenSDA USB port on the target board.

5.  Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control

6.  Connect the boards's RJ45 to network with Internet access (IP address to the board is assigned by the DHCP server). Make sure the connection on port 8883 is not blocked.

7.  Download the program to the target board.

8.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.


Prepare the Android application
The Android application requires Cognito service to authorize to AWS IoT in order to access device shadows. Use Amazon Cognito to create a new identity pool:

1.  In the Amazon Cognito Console https://console.aws.amazon.com/cognito/ select "Manage Federated Identities" and "Create new identity pool".

2.  Ensure Enable access to unauthenticated identities is checked. This allows the sample application to assume the unauthenticated role associated with this identity pool.
    Note: to keep this example simple it makes use of unauthenticated users in the identity pool. This can be used for getting started and prototypes but unauthenticated users should typically only be given read-only permissions in production applications. More information on Cognito identity pools including the Cognito developer guide can be found here: http://aws.amazon.com/cognito/.

3.  To obtain the Pool ID constant, select "Edit identity pool" and copy Identity pool ID (it will look like <REGION>:<ID>). This Pool ID together with region will be used in the application (policy and configuration file).

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
                    "arn:aws:iot:<REGION>:<ACCOUNT ID>:topic/$aws/things/<THING NAME>/shadow/get",
                    "arn:aws:iot:<REGION>:<ACCOUNT ID>:topic/my/things/<THING NAME>/updateAccel"
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
    cognito_pool_id=<REGION>:<COGNITO POOL ID>
    thing_name=<THING NAME>
    region=<REGION>

    Then move properties file into your Android device (application will ask for properties file though file browser dialog during first run).

8.  To run Android application do either:
    a) install and run pre-build apk on Android device (<SDK_Repository>\boards\src\aws_examples\remote_control\android\AwsRemoteControl.apk)
    b) open project in Android Studio, build it, attach Android device and Run application

    Application requires at least Android version 5.1 (Android SDK 22).

    Then in both cases when asked select AwsRemoteControlPreferences.properties file with AWS IoT preferences. Then application will establish MQTT connection to AWS server, download last state of thing's shadow and will be ready for user input.


Running the demo
================
The log below shows the output of the demo in the terminal window. The log can be different based on on the actions, which you have done in the Android application.

Android application displays Accelerometer data for all three axis and status of LEDs which is split into Desired and Reported section. Desired value is value wanted by user and Reported value is actual value of LEDs on device.

- When you turn on/off some of RGB LED in the Android application, the change should be visible on RGB LED on the board.
- When you refresh accelerometer data on Android application, you should see changes in the accelerometer data.

Every mentioned action takes approximately 1-3 seconds.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
0 0 [Tmr Svc] Starting key provisioning...
1 0 [Tmr Svc] Write root certificate...
2 0 [Tmr Svc] Write device private key...
3 74 [Tmr Svc] Write device certificate...
4 145 [Tmr Svc] Key provisioning done...
5 18315 [Tmr Svc] Getting IP address from DHCP ...
6 25315 [Tmr Svc] IPv4 Address: 10.42.0.198
7 25315 [Tmr Svc] DHCP OK
8 25321 [AWS-RemoteCtrl] [Shadow 0] MQTT: Creation of dedicated MQTT client succeeded.
9 25328 [AWS-RemoteCtrl] Sending command to MQTT task.
10 25333 [MQTT] Received message 10000 from queue.
11 32703 [MQTT] MQTT Connect was accepted. Connection established.
12 32703 [MQTT] Notifying task.
13 32711 [AWS-RemoteCtrl] Command sent to MQTT task passed.
14 32717 [AWS-RemoteCtrl] [Shadow 0] MQTT: Connect succeeded.
15 32722 [AWS-RemoteCtrl] Sending command to MQTT task.
16 32727 [MQTT] Received message 20000 from queue.
17 33133 [MQTT] MQTT Subscribe was accepted. Subscribed.
18 33133 [MQTT] Notifying task.
19 33140 [AWS-RemoteCtrl] Command sent to MQTT task passed.
20 33146 [AWS-RemoteCtrl] [Shadow 0] MQTT: Subscribe to callback topic succeeded.
21 33153 [AWS-RemoteCtrl] Sending command to MQTT task.
22 33158 [MQTT] Received message 30000 from queue.
23 33462 [MQTT] MQTT Subscribe was accepted. Subscribed.
24 33462 [MQTT] Notifying task.
25 33469 [AWS-RemoteCtrl] Command sent to MQTT task passed.
26 33475 [AWS-RemoteCtrl] [Shadow 0] MQTT: Subscribe to accepted topic succeeded.
27 33482 [AWS-RemoteCtrl] Sending command to MQTT task.
28 33487 [MQTT] Received message 40000 from queue.
29 33892 [MQTT] MQTT Subscribe was accepted. Subscribed.
30 33892 [MQTT] Notifying task.
31 33899 [AWS-RemoteCtrl] Command sent to MQTT task passed.
32 33905 [AWS-RemoteCtrl] [Shadow 0] MQTT: Subscribe to rejected topic succeeded.
33 33912 [AWS-RemoteCtrl] Sending command to MQTT task.
34 33917 [MQTT] Received message 50000 from queue.
35 33917 [MQTT] Notifying task.
36 33925 [AWS-RemoteCtrl] Command sent to MQTT task passed.
37 33930 [AWS-RemoteCtrl] [Shadow 0] MQTT: Publish to operation topic succeeded.
38 34322 [AWS-RemoteCtrl] Sending command to MQTT task.
39 34327 [MQTT] Received message 60000 from queue.
40 34934 [MQTT] MQTT Subscribe was accepted. Subscribed.
41 34934 [MQTT] Notifying task.
42 34941 [AWS-RemoteCtrl] Command sent to MQTT task passed.
43 34947 [AWS-RemoteCtrl] [Shadow 0] MQTT: Subscribe to accepted topic succeeded.
44 34954 [AWS-RemoteCtrl] Sending command to MQTT task.
45 34959 [MQTT] Received message 70000 from queue.
46 35364 [MQTT] MQTT Subscribe was accepted. Subscribed.
47 35364 [MQTT] Notifying task.
48 35371 [AWS-RemoteCtrl] Command sent to MQTT task passed.
49 35377 [AWS-RemoteCtrl] [Shadow 0] MQTT: Subscribe to rejected topic succeeded.
50 35384 [AWS-RemoteCtrl] Sending command to MQTT task.
51 35389 [MQTT] Received message 80000 from queue.
52 35389 [MQTT] Notifying task.
53 35397 [AWS-RemoteCtrl] Command sent to MQTT task passed.
54 35402 [AWS-RemoteCtrl] [Shadow 0] MQTT: Publish to operation topic succeeded.
55 35795 [AWS-RemoteCtrl] AWS Remote Control Demo initialized.
56 35800 [AWS-RemoteCtrl] Use mobile application to control the remote device.
57 50138 [AWS-RemoteCtrl] Turn on LED Red
58 50142 [AWS-RemoteCtrl] Sending command to MQTT task.
59 50147 [MQTT] Received message 90000 from queue.
60 50147 [MQTT] Notifying task.
61 50155 [AWS-RemoteCtrl] Command sent to MQTT task passed.
62 50160 [AWS-RemoteCtrl] [Shadow 0] MQTT: Publish to operation topic succeeded.
63 55501 [AWS-RemoteCtrl] [Shadow 0] MQTT: Return MQTT buffer succeeded.
64 77218 [AWS-RemoteCtrl] Update accelerometer.
65 77224 [AWS-RemoteCtrl] Sending command to MQTT task.
66 77229 [MQTT] Received message a0000 from queue.
67 77229 [MQTT] Notifying task.
68 77237 [AWS-RemoteCtrl] Command sent to MQTT task passed.
69 77243 [AWS-RemoteCtrl] [Shadow 0] MQTT: Publish to operation topic succeeded.
70 77250 [AWS-RemoteCtrl] [Shadow 0] MQTT: Return MQTT buffer succeeded.
71 88141 [AWS-RemoteCtrl] Turn off LED Red
72 88145 [AWS-RemoteCtrl] Sending command to MQTT task.
73 88150 [MQTT] Received message b0000 from queue.
74 88150 [MQTT] Notifying task.
75 88157 [AWS-RemoteCtrl] Command sent to MQTT task passed.
76 88163 [AWS-RemoteCtrl] [Shadow 0] MQTT: Publish to operation topic succeeded.
77 88252 [AWS-RemoteCtrl] Update accelerometer.
78 88258 [AWS-RemoteCtrl] Sending command to MQTT task.
79 88263 [MQTT] Received message c0000 from queue.
80 88263 [MQTT] Notifying task.
81 88270 [AWS-RemoteCtrl] Command sent to MQTT task passed.
82 88276 [AWS-RemoteCtrl] [Shadow 0] MQTT: Publish to operation topic succeeded.
83 89476 [AWS-RemoteCtrl] [Shadow 0] MQTT: Return MQTT buffer succeeded.
.
.
.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

