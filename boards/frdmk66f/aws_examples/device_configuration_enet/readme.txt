Overview
========
This example demonstrates how the aws iot device can be configured by mobile application.



Toolchain supported
===================
- GCC ARM Embedded  8.2.1
- IAR embedded Workbench  8.32.3
- Keil MDK  5.27
- MCUXpresso  11.0.0

Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-K66F board
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
5 5615 [Tmr Svc] Getting IP address from DHCP ...
6 11617 [Tmr Svc] IPv4 Address: 10.42.0.198
7 11618 [Tmr Svc] DHCP OK
8 11628 [MainDemoTask] [Shadow 0] MQTT: Creation of dedicated MQTT client succeeded.
9 24223 [MainDemoTask] [Shadow 0] MQTT: Connect succeeded.
10 24543 [MainDemoTask] [Shadow 0] MQTT: Subscribe to accepted topic succeeded.
11 24865 [MainDemoTask] [Shadow 0] MQTT: Subscribe to rejected topic succeeded.
12 24880 [MainDemoTask] [Shadow 0] MQTT: Publish to operation topic succeeded.
13 25605 [MainDemoTask] [Shadow 0] MQTT: Unsubscribe from rejected topic succeeded.
14 25928 [MainDemoTask] [Shadow 0] MQTT: Subscribe to callback topic succeeded.
15 25937 [MainDemoTask] Shadow client initialized.
16 25948 [ShDemoUpdt] Performing Thing Shadow update.
17 26265 [ShDemoUpdt] [Shadow 0] MQTT: Subscribe to accepted topic succeeded.
18 26579 [ShDemoUpdt] [Shadow 0] MQTT: Subscribe to rejected topic succeeded.
19 26586 [ShDemoUpdt] [Shadow 0] MQTT: Publish to operation topic succeeded.
20 27000 [ShDemoUpdt] Successfully performed update.
21 27002 [Shd-IOT-0] Shd-IOT-0 changing desired state.
22 27005 [ShDemoUpdt] Performing Thing Shadow update.
23 27013 [ShDemoUpdt] [Shadow 0] MQTT: Publish to operation topic succeeded.
24 27527 [ShDemoUpdt] Successfully performed update.
25 27529 [Shd-IOT-0] Shd-IOT-0 done changing desired state.
26 32002 [Shd-IOT-0] Shd-IOT-0 changing desired state.
27 32005 [ShDemoUpdt] Performing Thing Shadow update.
28 32014 [ShDemoUpdt] [Shadow 0] MQTT: Publish to operation topic succeeded.
29 32325 [MQTT] Successfully added new reported state to update queue.
30 32334 [ShDemoUpdt] Successfully performed update.
31 32336 [Shd-IOT-0] Shd-IOT-0 done changing desired state.
32 32337 [ShDemoUpdt] Performing Thing Shadow update.
33 32345 [ShDemoUpdt] [Shadow 0] MQTT: Publish to operation topic succeeded.
34 32859 [ShDemoUpdt] Successfully performed update.
35 37002 [Shd-IOT-0] Shd-IOT-0 changing desired state.
36 37005 [ShDemoUpdt] Performing Thing Shadow update.
37 37014 [ShDemoUpdt] [Shadow 0] MQTT: Publish to operation topic succeeded.
38 37331 [MQTT] Successfully added new reported state to update queue.
39 37333 [ShDemoUpdt] Successfully performed update.
41 37337 [Shd-IOT-0] Shd-IOT-0 done changing desired state.
40 37335 [ShDemoUpdt] Performing Thing Shadow update.
42 37344 [ShDemoUpdt] [Shadow 0] MQTT: Publish to operation topic succeeded.
43 37858 [ShDemoUpdt] Successfully performed update.
44 42002 [Shd-IOT-0] Shd-IOT-0 changing desired state.
45 42005 [ShDemoUpdt] Performing Thing Shadow update.
46 42014 [ShDemoUpdt] [Shadow 0] MQTT: Publish to operation topic succeeded.
47 42329 [MQTT] Successfully added new reported state to update queue.
48 42331 [ShDemoUpdt] Successfully performed update.
50 42335 [Shd-IOT-0] Shd-IOT-0 done changing desired state.
49 42333 [ShDemoUpdt] Performing Thing Shadow update.
51 42342 [ShDemoUpdt] [Shadow 0] MQTT: Publish to operation topic succeeded.
52 42856 [ShDemoUpdt] Successfully performed update.
53 47002 [Shd-IOT-0] Shd-IOT-0 changing desired state.
54 47005 [ShDemoUpdt] Performing Thing Shadow update.
55 47014 [ShDemoUpdt] [Shadow 0] MQTT: Publish to operation topic succeeded.
56 47327 [MQTT] Successfully added new reported state to update queue.
57 47336 [ShDemoUpdt] Successfully performed update.
58 47338 [Shd-IOT-0] Shd-IOT-0 done changing desired state.
59 47339 [ShDemoUpdt] Performing Thing Shadow update.
60 47347 [ShDemoUpdt] [Shadow 0] MQTT: Publish to operation topic succeeded.
61 47861 [ShDemoUpdt] Successfully performed update.
.
.
.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

