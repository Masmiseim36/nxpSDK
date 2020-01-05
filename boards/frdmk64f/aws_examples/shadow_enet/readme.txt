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

3.  This demo doesn't need WiFi network, you can leave the following macros from "aws_clientcredential.h" unmodified.
        #define clientcredentialWIFI_SSID       "Paste WiFi SSID here."
        #define clientcredentialWIFI_PASSWORD   "Paste WiFi password here."

4.  Make sure you have Internet access.

5.  Open example's project and build it.

6.  Connect a USB cable between the PC host and the OpenSDA port on the target board.

7.  Download the program to the target board with the CMSIS-DAP or J-Link debugger.

8.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

9.  Open a serial terminal on PC for serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control

Running the demo
================
The log below shows the output of the demo in the terminal window. The log can be different based on your local network configuration.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Initializing PHY...
0 159 [Tmr Svc] Write certificate...
1 463 [Tmr Svc] Device credential provisioning succeeded.
2 2105 [Tmr Svc] Getting IP address from DHCP ...
3 5105 [Tmr Svc] IPv4 Address: 192.168.2.102
4 5105 [Tmr Svc] DHCP OK
5 5108 [iot_thread] [INFO ][INIT][5108] SDK successfully initialized.
6 5108 [iot_thread] [INFO ][DEMO][5108] Successfully initialized the demo. Network type for the demo: 4
7 5108 [iot_thread] [INFO ][MQTT][5108] MQTT library successfully initialized.
8 5108 [iot_thread] [INFO ][Shadow][5108] Shadow library successfully initialized.
9 5108 [iot_thread] [INFO ][DEMO][5108] Shadow Thing Name is aws_rpr_michal (length 14).
10 10070 [iot_thread] [INFO ][MQTT][10070] Establishing new MQTT connection.
11 10074 [iot_thread] [INFO ][MQTT][10072] Anonymous metrics (SDK language, SDK version) will be provided to AWS IoT. Recompile with AWS_IOT_MQTT_ENABLE_METRICS set to 0 to disable.
12 10074 [iot_thread] [INFO ][MQTT][10074] (MQTT connection 20005c30, CONNECT operation 2000a438) Waiting for operation completion.
13 10278 [iot_thread] [INFO ][MQTT][10278] (MQTT connection 20005c30, CONNECT operation 2000a438) Wait complete with result SUCCESS.
14 10280 [iot_thread] [INFO ][MQTT][10280] New MQTT connection 2000819c established.
15 10280 [iot_thread] [INFO ][Shadow][10280] (aws_rpr_michal) Modifying Shadow DELTA callback.
16 10280 [iot_thread] [INFO ][Shadow][10280] (aws_rpr_michal) Adding new DELTA callback.
17 10280 [iot_thread] [INFO ][MQTT][10280] (MQTT connection 20005c30) SUBSCRIBE operation scheduled.
18 10280 [iot_thread] [INFO ][MQTT][10280] (MQTT connection 20005c30, SUBSCRIBE operation 2000aab8) Waiting for operation completion.
19 10440 [iot_thread] [INFO ][MQTT][10440] (MQTT connection 20005c30, SUBSCRIBE operation 2000aab8) Wait complete with result SUCCESS.
20 10441 [iot_thread] [INFO ][Shadow][10441] (aws_rpr_michal) Shadow DELTA callback operation complete with result SUCCESS.
21 10441 [iot_thread] [INFO ][Shadow][10441] (aws_rpr_michal) Modifying Shadow UPDATED callback.
22 10441 [iot_thread] [INFO ][Shadow][10441] (aws_rpr_michal) Adding new UPDATED callback.
23 10441 [iot_thread] [INFO ][MQTT][10441] (MQTT connection 20005c30) SUBSCRIBE operation scheduled.
24 10441 [iot_thread] [INFO ][MQTT][10441] (MQTT connection 20005c30, SUBSCRIBE operation 2000a438) Waiting for operation completion.
25 10599 [iot_thread] [INFO ][MQTT][10599] (MQTT connection 20005c30, SUBSCRIBE operation 2000a438) Wait complete with result SUCCESS.
26 10600 [iot_thread] [INFO ][Shadow][10600] (aws_rpr_michal) Shadow UPDATED callback operation complete with result SUCCESS.
27 10600 [iot_thread] [INFO ][MQTT][10600] (MQTT connection 20005c30) SUBSCRIBE operation scheduled.
28 10600 [iot_thread] [INFO ][MQTT][10600] (MQTT connection 20005c30, SUBSCRIBE operation 2000a918) Waiting for operation completion.
29 10785 [iot_thread] [INFO ][MQTT][10785] (MQTT connection 20005c30, SUBSCRIBE operation 2000a918) Wait complete with result SUCCESS.
30 10785 [iot_thread] [INFO ][MQTT][10785] (MQTT connection 20005c30) SUBSCRIBE operation scheduled.
31 10785 [iot_thread] [INFO ][MQTT][10785] (MQTT connection 20005c30, SUBSCRIBE operation 2000a548) Waiting for operation completion.
32 10944 [iot_thread] [INFO ][MQTT][10943] (MQTT connection 20005c30, SUBSCRIBE operation 2000a548) Wait complete with result SUCCESS.
33 10945 [iot_thread] [INFO ][MQTT][10945] (MQTT connection 20005c30) MQTT PUBLISH operation queued.
34 11131 [iot_thread] [INFO ][Shadow][11131] Shadow DELETE of aws_rpr_michal was ACCEPTED.
35 11132 [iot_thread] [INFO ][MQTT][11132] (MQTT connection 20005c30) UNSUBSCRIBE operation scheduled.
36 11132 [iot_thread] [INFO ][MQTT][11132] (MQTT connection 20005c30, UNSUBSCRIBE operation 2000a548) Waiting for operation completion.
37 11434 [iot_thread] [INFO ][MQTT][11434] (MQTT connection 20005c30, UNSUBSCRIBE operation 2000a548) Wait complete with result SUCCESS.
38 11435 [iot_thread] [INFO ][MQTT][11435] (MQTT connection 20005c30) UNSUBSCRIBE operation scheduled.
39 11435 [iot_thread] [INFO ][MQTT][11435] (MQTT connection 20005c30, UNSUBSCRIBE operation 2000a548) Waiting for operation completion.
40 11594 [iot_thread] [INFO ][MQTT][11594] (MQTT connection 20005c30, UNSUBSCRIBE operation 2000a548) Wait complete with result SUCCESS.
41 11596 [iot_thread] [INFO ][DEMO][11596] Successfully cleared Shadow of aws_rpr_michal.
42 11596 [iot_thread] [INFO ][DEMO][11596] Sending Shadow update 1 of 20: {"state":{"desired":{"powerOn":1}},"clientToken":"011596"}
43 11596 [iot_thread] [INFO ][MQTT][11596] (MQTT connection 20005c30) SUBSCRIBE operation scheduled.
44 11596 [iot_thread] [INFO ][MQTT][11596] (MQTT connection 20005c30, SUBSCRIBE operation 2000aa20) Waiting for operation completion.
45 11759 [iot_thread] [INFO ][MQTT][11759] (MQTT connection 20005c30, SUBSCRIBE operation 2000aa20) Wait complete with result SUCCESS.
46 11759 [iot_thread] [INFO ][MQTT][11759] (MQTT connection 20005c30) SUBSCRIBE operation scheduled.
47 11759 [iot_thread] [INFO ][MQTT][11759] (MQTT connection 20005c30, SUBSCRIBE operation 2000a548) Waiting for operation completion.
48 11921 [iot_thread] [INFO ][MQTT][11921] (MQTT connection 20005c30, SUBSCRIBE operation 2000a548) Wait complete with result SUCCESS.
49 11922 [iot_thread] [INFO ][MQTT][11922] (MQTT connection 20005c30) MQTT PUBLISH operation queued.
50 12112 [iot_thread] [INFO ][DEMO][12112] aws_rpr_michal changing state from 0 to 1.
51 12113 [iot_thread] [INFO ][MQTT][12113] (MQTT connection 20005c30) MQTT PUBLISH operation queued.
52 12113 [iot_thread] [INFO ][DEMO][12113] aws_rpr_michal sent new state report.
53 12115 [iot_thread] [INFO ][DEMO][12115] Shadow was updated!
Previous: {"state":{}}
Current:  {"state":{"desired":{"powerOn":1}}}
54 12116 [iot_thread] [INFO ][Shadow][12116] Shadow UPDATE of aws_rpr_michal was ACCEPTED.
55 12117 [iot_thread] [INFO ][DEMO][12117] Successfully sent Shadow update 1 of 20.
56 12444 [iot_thread] [INFO ][Shadow][12444] Shadow UPDATE of aws_rpr_michal was ACCEPTED.
57 12447 [iot_thread] [INFO ][DEMO][12447] Shadow was updated!
Previous: {"state":{"desired":{"powerOn":1}}}
Current:  {"state":{"desired":{"powerOn":1},"reported":{"powerOn":1}}}
58 15117 [iot_thread] [INFO ][DEMO][15117] Sending Shadow update 2 of 20: {"state":{"desired":{"powerOn":0}},"clientToken":"015117"}
59 15117 [iot_thread] [INFO ][MQTT][15117] (MQTT connection 20005c30) MQTT PUBLISH operation queued.
60 15311 [iot_thread] [INFO ][DEMO][15311] Shadow was updated!
Previous: {"state":{"desired":{"powerOn":1},"reported":{"powerOn":1}}}
Current:  {"state":{"desired":{"powerOn":0},"reported":{"powerOn":1}}}
61 15312 [iot_thread] [INFO ][DEMO][15312] aws_rpr_michal changing state from 1 to 0.
62 15313 [iot_thread] [INFO ][MQTT][15313] (MQTT connection 20005c30) MQTT PUBLISH operation queued.
63 15313 [iot_thread] [INFO ][DEMO][15313] aws_rpr_michal sent new state report.
64 15314 [iot_thread] [INFO ][Shadow][15314] Shadow UPDATE of aws_rpr_michal was ACCEPTED.
65 15315 [iot_thread] [INFO ][DEMO][15315] Successfully sent Shadow update 2 of 20.
66 15638 [iot_thread] [INFO ][DEMO][15638] Shadow was updated!
Previous: {"state":{"desired":{"powerOn":0},"reported":{"powerOn":1}}}
Current:  {"state":{"desired":{"powerOn":0},"reported":{"powerOn":0}}}
67 15640 [iot_thread] [INFO ][Shadow][15640] Shadow UPDATE of aws_rpr_michal was ACCEPTED.
68 18315 [iot_thread] [INFO ][DEMO][18315] Sending Shadow update 3 of 20: {"state":{"desired":{"powerOn":1}},"clientToken":"018315"}
69 18315 [iot_thread] [INFO ][MQTT][18315] (MQTT connection 20005c30) MQTT PUBLISH operation queued.
70 18505 [iot_thread] [INFO ][Shadow][18505] Shadow UPDATE of aws_rpr_michal was ACCEPTED.
71 18506 [iot_thread] [INFO ][DEMO][18506] Successfully sent Shadow update 3 of 20.
.
.
.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

