Overview
========
The simple Shadow lightbulb example to illustrate how client application and things communicate with the Shadow service.


Toolchain supported
===================
- Keil MDK 5.24a
- IAR embedded Workbench 8.22.2
- GCC ARM Embedded 7-2017-q4-major
- MCUXpresso10.2.0

Hardware requirements
=====================

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

5.  Connect a USB cable between the PC host and the OpenSDA port on the target board.

6.  Download the program to the target board with a J-Link debugger.

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
2 0 [Tmr Svc] Write device private key...
3 9 [Tmr Svc] Write device certificate...
4 19 [Tmr Svc] Key provisioning done...
5 19 [Tmr Svc] Starting WiFi...
6 1271 [Tmr Svc] WiFi module initialized.
7 4274 [Tmr Svc] WiFi connected to AP AndroidAP.
8 5278 [Tmr Svc] IP Address acquired 192.168.43.203
9 5284 [MainDemoTask] [Shadow 0] MQTT: Creation of dedicated MQTT client succeeded.
10 5291 [MainDemoTask] Sending command to MQTT task.
11 5296 [MQTT] Received message 10000 from queue.
12 5613 [MQTT] Looked up a254jqzk0kcvf0.iot.us-west-2.amazonaws.com as 52.41.156.217
13 11915 [MQTT] MQTT Connect was accepted. Connection established.
14 11915 [MQTT] Notifying task.
15 11924 [MainDemoTask] Command sent to MQTT task passed.
16 11929 [MainDemoTask] [Shadow 0] MQTT: Connect succeeded.
17 11934 [MainDemoTask] Sending command to MQTT task.
18 12915 [MQTT] Received message 20000 from queue.
19 14201 [MQTT] MQTT Subscribe was accepted. Subscribed.
20 14201 [MQTT] Notifying task.
21 14209 [MainDemoTask] Command sent to MQTT task passed.
22 14214 [MainDemoTask] [Shadow 0] MQTT: Subscribe to callback topic succeeded.
23 14221 [MainDemoTask] Sending command to MQTT task.
24 15201 [MQTT] Received message 30000 from queue.
25 16676 [MQTT] MQTT Subscribe was accepted. Subscribed.
26 16676 [MQTT] Notifying task.
27 16684 [MainDemoTask] Command sent to MQTT task passed.
28 16689 [MainDemoTask] [Shadow 0] MQTT: Subscribe to callback topic succeeded.
29 16696 [MainDemoTask] Sending command to MQTT task.
30 17676 [MQTT] Received message 40000 from queue.
31 18947 [MQTT] MQTT Subscribe was accepted. Subscribed.
32 18947 [MQTT] Notifying task.
33 18955 [MainDemoTask] Command sent to MQTT task passed.
34 18960 [MainDemoTask] [Shadow 0] MQTT: Subscribe to callback topic succeeded.
35 18967 [MainDemoTask] Sending command to MQTT task.
36 19947 [MQTT] Received message 50000 from queue.
37 21237 [MQTT] MQTT Subscribe was accepted. Subscribed.
38 21237 [MQTT] Notifying task.
39 21245 [MainDemoTask] Command sent to MQTT task passed.
40 21250 [MainDemoTask] [Shadow 0] MQTT: Subscribe to accepted topic succeeded.
41 21257 [MainDemoTask] Sending command to MQTT task.
42 22237 [MQTT] Received message 60000 from queue.
43 23534 [MQTT] MQTT Subscribe was accepted. Subscribed.
44 23534 [MQTT] Notifying task.
45 23542 [MainDemoTask] Command sent to MQTT task passed.
46 23547 [MainDemoTask] [Shadow 0] MQTT: Subscribe to rejected topic succeeded.
47 23554 [MainDemoTask] Sending command to MQTT task.
48 24534 [MQTT] Received message 70000 from queue.
49 24534 [MQTT] Notifying task.
50 24542 [MainDemoTask] Command sent to MQTT task passed.
51 24547 [MainDemoTask] [Shadow 0] MQTT: Publish to operation topic succeeded.
52 25854 [Shd-IOT-0] Sending command to MQTT task.
53 26853 [MQTT] Received message 80000 from queue.
54 28123 [MQTT] MQTT Subscribe was accepted. Subscribed.
55 28123 [MQTT] Notifying task.
56 28131 [Shd-IOT-0] Command sent to MQTT task passed.
57 28136 [Shd-IOT-0] [Shadow 0] MQTT: Subscribe to accepted topic succeeded.
58 28143 [Shd-IOT-0] Sending command to MQTT task.
59 29123 [MQTT] Received message 90000 from queue.
60 30398 [MQTT] MQTT Subscribe was accepted. Subscribed.
61 30398 [MQTT] Notifying task.
62 30406 [Shd-IOT-0] Command sent to MQTT task passed.
63 30411 [Shd-IOT-0] [Shadow 0] MQTT: Subscribe to rejected topic succeeded.
64 30418 [Shd-IOT-0] Sending command to MQTT task.
65 31398 [MQTT] Received message a0000 from queue.
66 31399 [MQTT] Notifying task.
67 31406 [Shd-IOT-0] Command sent to MQTT task passed.
68 31411 [Shd-IOT-0] [Shadow 0] MQTT: Publish to operation topic succeeded.
69 32726 [Shd-IOT-0] Shadow Demo initialized.
70 32730 [Shd-IOT-0] Client request to change color of light bulb.
71 32736 [Shd-IOT-0] Sending command to MQTT task.
72 33726 [MQTT] Received message b0000 from queue.
73 33727 [MQTT] Notifying task.
74 33734 [Shd-IOT-0] Command sent to MQTT task passed.
75 33739 [Shd-IOT-0] [Shadow 0] MQTT: Publish to operation topic succeeded.
76 35176 [ShDemoUpdt] Performing thing update.
77 35181 [Shd-IOT-0] Client change done in thing shadow.
78 35187 [ShDemoUpdt] Sending command to MQTT task.
79 36177 [MQTT] Received message c0000 from queue.
80 36178 [MQTT] Notifying task.
81 36185 [ShDemoUpdt] Command sent to MQTT task passed.
82 36190 [ShDemoUpdt] [Shadow 0] MQTT: Publish to operation topic succeeded.
83 37524 [ShDemoUpdt] Performing thing update complete.
84 37726 [Shd-IOT-0] Client request to change color of light bulb.
85 37732 [Shd-IOT-0] Sending command to MQTT task.
86 38524 [MQTT] Received message d0000 from queue.
87 38525 [MQTT] Notifying task.
88 38532 [Shd-IOT-0] Command sent to MQTT task passed.
89 38537 [Shd-IOT-0] [Shadow 0] MQTT: Publish to operation topic succeeded.
90 39827 [Shd-IOT-0] Client change done in thing shadow.
91 39833 [ShDemoUpdt] Performing thing update.
92 39838 [ShDemoUpdt] Sending command to MQTT task.
93 40828 [MQTT] Received message e0000 from queue.
94 40829 [MQTT] Notifying task.
95 40836 [ShDemoUpdt] Command sent to MQTT task passed.
96 40841 [ShDemoUpdt] [Shadow 0] MQTT: Publish to operation topic succeeded.
97 42150 [ShDemoUpdt] Performing thing update complete.
.
.
.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

