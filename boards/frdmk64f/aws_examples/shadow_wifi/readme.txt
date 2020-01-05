Overview
========
The simple Shadow lightbulb example to illustrate how client application and things communicate with the Shadow service.


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

5.  Connect a USB cable between the PC host and the OpenSDA port on the target board.

6.  Download the program to the target board with the CMSIS-DAP or J-Link debugger.

7.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

8.  Open a serial terminal on PC for serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control

Running the demo
================
The log below shows the output of the demo in the terminal window. The log can be different based on your WiFi network configuration.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
0 160 [Tmr Svc] Write certificate...
1 463 [Tmr Svc] Device credential provisioning succeeded.
2 466 [iot_thread] [INFO ][INIT][466] SDK successfully initialized.
3 2344 [iot_thread] [INFO ][DEMO][2344] Successfully initialized the demo. Network type for the demo: 1
4 2344 [iot_thread] [INFO ][MQTT][2344] MQTT library successfully initialized.
5 2344 [iot_thread] [INFO ][Shadow][2344] Shadow library successfully initialized.
6 2344 [iot_thread] [INFO ][DEMO][2344] Shadow Thing Name is aws_rpr_michal (length 14).
8 7133 [iot_thread] [INFO ][MQTT][7133] Establishing new MQTT connection.
9 7135 [iot_thread] [INFO ][MQTT][7135] Anonymous metrics (SDK language, SDK version) will be provided to AWS IoT. Recompile with AWS_IOT_MQTT_ENABLE_METRICS set to 0 to disable.
10 7136 [iot_thread] [INFO ][MQTT][7136] (MQTT connection 20008910, CONNECT operation 20008a28) Waiting for operation completion.
11 7351 [iot_thread] [INFO ][MQTT][7351] (MQTT connection 20008910, CONNECT operation 20008a28) Wait complete with result SUCCESS.
12 7352 [iot_thread] [INFO ][MQTT][7352] New MQTT connection 20003914 established.
13 7352 [iot_thread] [INFO ][Shadow][7352] (aws_rpr_michal) Modifying Shadow DELTA callback.
14 7352 [iot_thread] [INFO ][Shadow][7352] (aws_rpr_michal) Adding new DELTA callback.
15 7352 [iot_thread] [INFO ][MQTT][7352] (MQTT connection 20008910) SUBSCRIBE operation scheduled.
16 7352 [iot_thread] [INFO ][MQTT][7352] (MQTT connection 20008910, SUBSCRIBE operation 200082b0) Waiting for operation completion.
17 7514 [iot_thread] [INFO ][MQTT][7514] (MQTT connection 20008910, SUBSCRIBE operation 200082b0) Wait complete with result SUCCESS.
18 7515 [iot_thread] [INFO ][Shadow][7515] (aws_rpr_michal) Shadow DELTA callback operation complete with result SUCCESS.
19 7515 [iot_thread] [INFO ][Shadow][7515] (aws_rpr_michal) Modifying Shadow UPDATED callback.
20 7515 [iot_thread] [INFO ][Shadow][7515] (aws_rpr_michal) Adding new UPDATED callback.
21 7515 [iot_thread] [INFO ][MQTT][7515] (MQTT connection 20008910) SUBSCRIBE operation scheduled.
22 7515 [iot_thread] [INFO ][MQTT][7515] (MQTT connection 20008910, SUBSCRIBE operation 200082b0) Waiting for operation completion.
23 7677 [iot_thread] [INFO ][MQTT][7677] (MQTT connection 20008910, SUBSCRIBE operation 200082b0) Wait complete with result SUCCESS.
24 7678 [iot_thread] [INFO ][Shadow][7678] (aws_rpr_michal) Shadow UPDATED callback operation complete with result SUCCESS.
25 7678 [iot_thread] [INFO ][MQTT][7678] (MQTT connection 20008910) SUBSCRIBE operation scheduled.
26 7678 [iot_thread] [INFO ][MQTT][7678] (MQTT connection 20008910, SUBSCRIBE operation 20008d68) Waiting for operation completion.
27 7846 [iot_thread] [INFO ][MQTT][7846] (MQTT connection 20008910, SUBSCRIBE operation 20008d68) Wait complete with result SUCCESS.
28 7846 [iot_thread] [INFO ][MQTT][7846] (MQTT connection 20008910) SUBSCRIBE operation scheduled.
29 7846 [iot_thread] [INFO ][MQTT][7846] (MQTT connection 20008910, SUBSCRIBE operation 20008a28) Waiting for operation completion.
30 8008 [iot_thread] [INFO ][MQTT][8008] (MQTT connection 20008910, SUBSCRIBE operation 20008a28) Wait complete with result SUCCESS.
31 8009 [iot_thread] [INFO ][MQTT][8009] (MQTT connection 20008910) MQTT PUBLISH operation queued.
32 8201 [iot_thread] [INFO ][Shadow][8201] Shadow DELETE of aws_rpr_michal was ACCEPTED.
33 8202 [iot_thread] [INFO ][MQTT][8202] (MQTT connection 20008910) UNSUBSCRIBE operation scheduled.
34 8202 [iot_thread] [INFO ][MQTT][8202] (MQTT connection 20008910, UNSUBSCRIBE operation 20008a28) Waiting for operation completion.
35 8363 [iot_thread] [INFO ][MQTT][8363] (MQTT connection 20008910, UNSUBSCRIBE operation 20008a28) Wait complete with result SUCCESS.
36 8363 [iot_thread] [INFO ][MQTT][8363] (MQTT connection 20008910) UNSUBSCRIBE operation scheduled.
37 8363 [iot_thread] [INFO ][MQTT][8363] (MQTT connection 20008910, UNSUBSCRIBE operation 20008a28) Waiting for operation completion.
38 8525 [iot_thread] [INFO ][MQTT][8525] (MQTT connection 20008910, UNSUBSCRIBE operation 20008a28) Wait complete with result SUCCESS.
39 8526 [iot_thread] [INFO ][DEMO][8526] Successfully cleared Shadow of aws_rpr_michal.
40 8526 [iot_thread] [INFO ][DEMO][8526] Sending Shadow update 1 of 20: {"state":{"desired":{"powerOn":1}},"clientToken":"008526"}
41 8526 [iot_thread] [INFO ][MQTT][8526] (MQTT connection 20008910) SUBSCRIBE operation scheduled.
42 8526 [iot_thread] [INFO ][MQTT][8526] (MQTT connection 20008910, SUBSCRIBE operation 20008a68) Waiting for operation completion.
43 8682 [iot_thread] [INFO ][MQTT][8682] (MQTT connection 20008910, SUBSCRIBE operation 20008a68) Wait complete with result SUCCESS.
44 8682 [iot_thread] [INFO ][MQTT][8682] (MQTT connection 20008910) SUBSCRIBE operation scheduled.
45 8682 [iot_thread] [INFO ][MQTT][8682] (MQTT connection 20008910, SUBSCRIBE operation 20008a68) Waiting for operation completion.
46 8838 [iot_thread] [INFO ][MQTT][8838] (MQTT connection 20008910, SUBSCRIBE operation 20008a68) Wait complete with result SUCCESS.
47 8839 [iot_thread] [INFO ][MQTT][8839] (MQTT connection 20008910) MQTT PUBLISH operation queued.
48 9032 [iot_thread] [INFO ][DEMO][9032] aws_rpr_michal changing state from 0 to 1.
49 9032 [iot_thread] [INFO ][MQTT][9032] (MQTT connection 20008910) MQTT PUBLISH operation queued.
50 9033 [iot_thread] [INFO ][DEMO][9033] aws_rpr_michal sent new state report.
51 9040 [iot_thread] [INFO ][Shadow][9040] Shadow UPDATE of aws_rpr_michal was ACCEPTED.
52 9043 [iot_thread] [INFO ][DEMO][9043] Shadow was updated!
Previous: {"state":{}}
Current:  {"state":{"desired":{"powerOn":1}}}
53 9043 [iot_thread] [INFO ][DEMO][9043] Successfully sent Shadow update 1 of 20.
54 9224 [iot_thread] [INFO ][Shadow][9224] Shadow UPDATE of aws_rpr_michal was ACCEPTED.
55 9228 [iot_thread] [INFO ][DEMO][9228] Shadow was updated!
Previous: {"state":{"desired":{"powerOn":1}}}
Current:  {"state":{"desired":{"powerOn":1},"reported":{"powerOn":1}}}
56 12043 [iot_thread] [INFO ][DEMO][12043] Sending Shadow update 2 of 20: {"state":{"desired":{"powerOn":0}},"clientToken":"012043"}
57 12045 [iot_thread] [INFO ][MQTT][12043] (MQTT connection 20008910) MQTT PUBLISH operation queued.
58 12232 [iot_thread] [INFO ][Shadow][12232] Shadow UPDATE of aws_rpr_michal was ACCEPTED.
59 12233 [iot_thread] [INFO ][DEMO][12233] Successfully sent Shadow update 2 of 20.
60 12235 [iot_thread] [INFO ][DEMO][12235] Shadow was updated!
Previous: {"state":{"desired":{"powerOn":1},"reported":{"powerOn":1}}}
Current:  {"state":{"desired":{"powerOn":0},"reported":{"powerOn":1}}}
61 12238 [iot_thread] [INFO ][DEMO][12238] aws_rpr_michal changing state from 1 to 0.
62 12238 [iot_thread] [INFO ][MQTT][12238] (MQTT connection 20008910) MQTT PUBLISH operation queued.
63 12238 [iot_thread] [INFO ][DEMO][12238] aws_rpr_michal sent new state report.
64 12431 [iot_thread] [INFO ][DEMO][12431] Shadow was updated!
Previous: {"state":{"desired":{"powerOn":0},"reported":{"powerOn":1}}}
Current:  {"state":{"desired":{"powerOn":0},"reported":{"powerOn":0}}}
65 12445 [iot_thread] [INFO ][Shadow][12445] Shadow UPDATE of aws_rpr_michal was ACCEPTED.
66 15239 [iot_thread] [INFO ][DEMO][15239] Sending Shadow update 3 of 20: {"state":{"desired":{"powerOn":1}},"clientToken":"015239"}
67 15241 [iot_thread] [INFO ][MQTT][15239] (MQTT connection 20008910) MQTT PUBLISH operation queued.
68 15431 [iot_thread] [INFO ][Shadow][15431] Shadow UPDATE of aws_rpr_michal was ACCEPTED.
69 15432 [iot_thread] [INFO ][DEMO][15432] Successfully sent Shadow update 3 of 20.
.
.
.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

