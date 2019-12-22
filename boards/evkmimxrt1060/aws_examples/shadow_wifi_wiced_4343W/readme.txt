Overview
========
The simple Shadow lightbulb example to illustrate how client application and things communicate with the Shadow service.


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
0 120 [Tmr Svc] Write certificate...
1 246 [Tmr Svc] Device credential provisioning succeeded.
2 249 [iot_thread] [INFO ][INIT][249] SDK successfully initialized.

 AsyncInterrupt is not supported 
WLAN MAC Address : A0:C9:A0:3D:EE:8F
WLAN Firmware    : wl0: Feb 12 2018 04:08:14 version 7.79.2 (r683798 CY) FWID 01-27b63357
WLAN CLM         : API: 12.2 Data: 9.10.39 Compiler: 1.29.4 ClmImport: 1.36.3 Creation: 2018-02-12 04:00:50 
3 4942 [iot_thread] [INFO ][DEMO][4942] Successfully initialized the demo. Network type for the demo: 1
4 4942 [iot_thread] [INFO ][MQTT][4942] MQTT library successfully initialized.
5 4942 [iot_thread] [INFO ][Shadow][4942] Shadow library successfully initialized.
6 4942 [iot_thread] [INFO ][DEMO][4942] Shadow Thing Name is aws_rpr_michal (length 14).
7 8124 [iot_thread] [INFO ][MQTT][8124] Establishing new MQTT connection.
8 8130 [iot_thread] [INFO ][MQTT][8130] Anonymous metrics (SDK language, SDK version) will be provided to AWS IoT. Recompile with AWS_IOT_MQTT_ENABLE_METRICS set to 0 to disable.
9 8131 [iot_thread] [INFO ][MQTT][8131] (MQTT connection 20208b50, CONNECT operation 20209ac8) Waiting for operation completion.
10 8323 [iot_thread] [INFO ][MQTT][8323] (MQTT connection 20208b50, CONNECT operation 20209ac8) Wait complete with result SUCCESS.
11 8324 [iot_thread] [INFO ][MQTT][8324] New MQTT connection 20204ae4 established.
12 8324 [iot_thread] [INFO ][Shadow][8324] (aws_rpr_michal) Modifying Shadow DELTA callback.
13 8324 [iot_thread] [INFO ][Shadow][8324] (aws_rpr_michal) Adding new DELTA callback.
14 8324 [iot_thread] [INFO ][MQTT][8324] (MQTT connection 20208b50) SUBSCRIBE operation scheduled.
15 8324 [iot_thread] [INFO ][MQTT][8324] (MQTT connection 20208b50, SUBSCRIBE operation 2020a278) Waiting for operation completion.
16 8480 [iot_thread] [INFO ][MQTT][8480] (MQTT connection 20208b50, SUBSCRIBE operation 2020a278) Wait complete with result SUCCESS.
17 8481 [iot_thread] [INFO ][Shadow][8481] (aws_rpr_michal) Shadow DELTA callback operation complete with result SUCCESS.
18 8481 [iot_thread] [INFO ][Shadow][8481] (aws_rpr_michal) Modifying Shadow UPDATED callback.
19 8481 [iot_thread] [INFO ][Shadow][8481] (aws_rpr_michal) Adding new UPDATED callback.
20 8481 [iot_thread] [INFO ][MQTT][8481] (MQTT connection 20208b50) SUBSCRIBE operation scheduled.
21 8481 [iot_thread] [INFO ][MQTT][8481] (MQTT connection 20208b50, SUBSCRIBE operation 20209ac8) Waiting for operation completion.
22 8640 [iot_thread] [INFO ][MQTT][8640] (MQTT connection 20208b50, SUBSCRIBE operation 20209ac8) Wait complete with result SUCCESS.
23 8641 [iot_thread] [INFO ][Shadow][8641] (aws_rpr_michal) Shadow UPDATED callback operation complete with result SUCCESS.
24 8641 [iot_thread] [INFO ][MQTT][8641] (MQTT connection 20208b50) SUBSCRIBE operation scheduled.
25 8641 [iot_thread] [INFO ][MQTT][8641] (MQTT connection 20208b50, SUBSCRIBE operation 2020a170) Waiting for operation completion.
26 8797 [iot_thread] [INFO ][MQTT][8797] (MQTT connection 20208b50, SUBSCRIBE operation 2020a170) Wait complete with result SUCCESS.
27 8797 [iot_thread] [INFO ][MQTT][8797] (MQTT connection 20208b50) SUBSCRIBE operation scheduled.
28 8798 [iot_thread] [INFO ][MQTT][8798] (MQTT connection 20208b50, SUBSCRIBE operation 2020a068) Waiting for operation completion.
29 8959 [iot_thread] [INFO ][MQTT][8959] (MQTT connection 20208b50, SUBSCRIBE operation 2020a068) Wait complete with result SUCCESS.
30 8961 [iot_thread] [INFO ][MQTT][8961] (MQTT connection 20208b50) MQTT PUBLISH operation queued.
31 9148 [iot_thread] [INFO ][Shadow][9148] Shadow DELETE of aws_rpr_michal was ACCEPTED.
32 9149 [iot_thread] [INFO ][MQTT][9149] (MQTT connection 20208b50) UNSUBSCRIBE operation scheduled.
33 9149 [iot_thread] [INFO ][MQTT][9149] (MQTT connection 20208b50, UNSUBSCRIBE operation 20209e80) Waiting for operation completion.
34 9453 [iot_thread] [INFO ][MQTT][9453] (MQTT connection 20208b50, UNSUBSCRIBE operation 20209e80) Wait complete with result SUCCESS.
35 9453 [iot_thread] [INFO ][MQTT][9453] (MQTT connection 20208b50) UNSUBSCRIBE operation scheduled.
36 9453 [iot_thread] [INFO ][MQTT][9453] (MQTT connection 20208b50, UNSUBSCRIBE operation 20209e20) Waiting for operation completion.
37 9629 [iot_thread] [INFO ][MQTT][9629] (MQTT connection 20208b50, UNSUBSCRIBE operation 20209e20) Wait complete with result SUCCESS.
38 9630 [iot_thread] [INFO ][DEMO][9630] Successfully cleared Shadow of aws_rpr_michal.
39 9630 [iot_thread] [INFO ][DEMO][9630] Sending Shadow update 1 of 20: {"state":{"desired":{"powerOn":1}},"clientToken":"009630"}
40 9630 [iot_thread] [INFO ][MQTT][9630] (MQTT connection 20208b50) SUBSCRIBE operation scheduled.
41 9630 [iot_thread] [INFO ][MQTT][9630] (MQTT connection 20208b50, SUBSCRIBE operation 2020a170) Waiting for operation completion.
42 9788 [iot_thread] [INFO ][MQTT][9788] (MQTT connection 20208b50, SUBSCRIBE operation 2020a170) Wait complete with result SUCCESS.
43 9788 [iot_thread] [INFO ][MQTT][9788] (MQTT connection 20208b50) SUBSCRIBE operation scheduled.
44 9788 [iot_thread] [INFO ][MQTT][9788] (MQTT connection 20208b50, SUBSCRIBE operation 2020a068) Waiting for operation completion.
45 9960 [iot_thread] [INFO ][MQTT][9960] (MQTT connection 20208b50, SUBSCRIBE operation 2020a068) Wait complete with result SUCCESS.
46 9961 [iot_thread] [INFO ][MQTT][9961] (MQTT connection 20208b50) MQTT PUBLISH operation queued.
47 10149 [iot_thread] [INFO ][Shadow][10149] Shadow UPDATE of aws_rpr_michal was ACCEPTED.
48 10150 [iot_thread] [INFO ][DEMO][10149] Shadow was updated!
Previous: {"state":{}}
Current:  {"state":{"desired":{"powerOn":1}}}
49 10151 [iot_thread] [INFO ][DEMO][10151] Successfully sent Shadow update 1 of 20.
50 10154 [iot_thread] [INFO ][DEMO][10154] aws_rpr_michal changing state from 0 to 1.
51 10154 [iot_thread] [INFO ][MQTT][10154] (MQTT connection 20208b50) MQTT PUBLISH operation queued.
52 10155 [iot_thread] [INFO ][DEMO][10154] aws_rpr_michal sent new state report.
53 10479 [iot_thread] [INFO ][DEMO][10479] Shadow was updated!
Previous: {"state":{"desired":{"powerOn":1}}}
Current:  {"state":{"desired":{"powerOn":1},"reported":{"powerOn":1}}}
54 10481 [iot_thread] [INFO ][Shadow][10481] Shadow UPDATE of aws_rpr_michal was ACCEPTED.
55 13156 [iot_thread] [INFO ][DEMO][13156] Sending Shadow update 2 of 20: {"state":{"desired":{"powerOn":0}},"clientToken":"013156"}
56 13158 [iot_thread] [INFO ][MQTT][13158] (MQTT connection 20208b50) MQTT PUBLISH operation queued.
57 13345 [iot_thread] [INFO ][DEMO][13345] aws_rpr_michal changing state from 1 to 0.
58 13346 [iot_thread] [INFO ][MQTT][13346] (MQTT connection 20208b50) MQTT PUBLISH operation queued.
59 13346 [iot_thread] [INFO ][DEMO][13346] aws_rpr_michal sent new state report.
60 13347 [iot_thread] [INFO ][DEMO][13347] Shadow was updated!
Previous: {"state":{"desired":{"powerOn":1},"reported":{"powerOn":1}}}
Current:  {"state":{"desired":{"powerOn":0},"reported":{"powerOn":1}}}
61 13349 [iot_thread] [INFO ][Shadow][13349] Shadow UPDATE of aws_rpr_michal was ACCEPTED.
62 13349 [iot_thread] [INFO ][DEMO][13349] Successfully sent Shadow update 2 of 20.
63 13675 [iot_thread] [INFO ][Shadow][13675] Shadow UPDATE of aws_rpr_michal was ACCEPTED.
64 13677 [iot_thread] [INFO ][DEMO][13677] Shadow was updated!
Previous: {"state":{"desired":{"powerOn":0},"reported":{"powerOn":1}}}
Current:  {"state":{"desired":{"powerOn":0},"reported":{"powerOn":0}}}
65 16351 [iot_thread] [INFO ][DEMO][16351] Sending Shadow update 3 of 20: {"state":{"desired":{"powerOn":1}},"clientToken":"016351"}
66 16353 [iot_thread] [INFO ][MQTT][16353] (MQTT connection 20208b50) MQTT PUBLISH operation queued.
67 16541 [iot_thread] [INFO ][Shadow][16541] Shadow UPDATE of aws_rpr_michal was ACCEPTED.
68 16542 [iot_thread] [INFO ][DEMO][16542] Shadow was updated!
Previous: {"state":{"desired":{"powerOn":0},"reported":{"powerOn":0}}}
Current:  {"state":{"desired":{"powerOn":1},"reported":{"powerOn":0}}}
69 16544 [iot_thread] [INFO ][DEMO][16544] Successfully sent Shadow update 3 of 20.
.
.
.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

