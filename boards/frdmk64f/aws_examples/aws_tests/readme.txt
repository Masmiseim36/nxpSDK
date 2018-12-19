Overview
========
The simple Shadow lightbulb example to illustrate how client application and things communicate with the Shadow service.

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
0 0 [Tmr Svc] Starting key provisioning...
1 0 [Tmr Svc] Write root certificate...
2 15 [Tmr Svc] Write device private key...
3 488 [Tmr Svc] Write device certificate...
4 508 [Tmr Svc] Key provisioning done...
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
- FRDM-K64F board
- GT202 Adaptor V1.04
- Personal Computer

Board settings
==============
Plug GT202 Adaptor board to FRDM stackable headers (J1, J2, J3, J4).
GT202 shield is compatible *only* with FRDM-K64 rev_E.
Previous revisions cannot provide GPIO mux on J2:2 (WLAN_PWRON).

Toolchain supported
===================
- IAR embedded Workbench  8.32.1
- Keil MDK  5.26
- GCC ARM Embedded  7.3.1
- MCUXpresso 10.3.0

