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
- Mini/micro USB cable
- EVKB-IMXRT1050 board
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
0 0 [Tmr Svc] Starting key provisioning...
1 0 [Tmr Svc] Write root certificate...
2 106 [Tmr Svc] Write device private key...
3 808 [Tmr Svc] Write device certificate...
4 890 [Tmr Svc] Key provisioning done...
5 2617 [Tmr Svc] Getting IP address from DHCP ...
6 5619 [Tmr Svc] IPv4 Address: 192.168.1.70
7 5620 [Tmr Svc] DHCP OK
8 5630 [MainDemoTask] [Shadow 0] MQTT: Creation of dedicated MQTT client succeeded.
9 5640 [MainDemoTask] Sending command to MQTT task.
10 5647 [MQTT] Received message 10000 from queue.
11 27859 [MQTT] MQTT Connect was accepted. Connection established.
12 27859 [MQTT] Notifying task.
13 27873 [MainDemoTask] Command sent to MQTT task passed.
14 27879 [MainDemoTask] [Shadow 0] MQTT: Connect succeeded.
15 27887 [MainDemoTask] Sending command to MQTT task.
16 27893 [MQTT] Received message 20000 from queue.
17 28124 [MQTT] MQTT Subscribe was accepted. Subscribed.
18 28124 [MQTT] Notifying task.
19 28137 [MainDemoTask] Command sent to MQTT task passed.
20 28143 [MainDemoTask] [Shadow 0] MQTT: Subscribe to accepted topic succeeded.
21 28153 [MainDemoTask] Sending command to MQTT task.
22 28159 [MQTT] Received message 30000 from queue.
23 28288 [MQTT] MQTT Subscribe was accepted. Subscribed.
24 28288 [MQTT] Notifying task.
25 28301 [MainDemoTask] Command sent to MQTT task passed.
26 28307 [MainDemoTask] [Shadow 0] MQTT: Subscribe to rejected topic succeeded.
27 28317 [MainDemoTask] Sending command to MQTT task.
28 28323 [MQTT] Received message 40000 from queue.
29 28338 [MQTT] Notifying task.
30 28349 [MainDemoTask] Command sent to MQTT task passed.
31 28355 [MainDemoTask] [Shadow 0] MQTT: Publish to operation topic succeeded.
32 28573 [MainDemoTask] Sending command to MQTT task.
33 28579 [MQTT] Received message 50000 from queue.
34 28808 [MQTT] MQTT Unsubscribe was successful.
35 28808 [MQTT] Notifying task.
36 28820 [MainDemoTask] Command sent to MQTT task passed.
37 28827 [MainDemoTask] [Shadow 0] MQTT: Unsubscribe from rejected topic succeeded.
38 28837 [MainDemoTask] Sending command to MQTT task.
39 28843 [MQTT] Received message 60000 from queue.
40 28972 [MQTT] MQTT Subscribe was accepted. Subscribed.
41 28972 [MQTT] Notifying task.
42 28985 [MainDemoTask] Command sent to MQTT task passed.
43 28991 [MainDemoTask] [Shadow 0] MQTT: Subscribe to callback topic succeeded.
44 29000 [MainDemoTask] Shadow client initialized.
45 29009 [ShDemoUpdt] Performing Thing Shadow update.
46 29010 [ShDemoUpdt] Sending command to MQTT task.
47 29012 [MQTT] Received message 70000 from queue.
48 29140 [MQTT] MQTT Subscribe was accepted. Subscribed.
49 29140 [MQTT] Notifying task.
50 29144 [ShDemoUpdt] Command sent to MQTT task passed.
51 29146 [ShDemoUpdt] [Shadow 0] MQTT: Subscribe to accepted topic succeeded.
52 29148 [ShDemoUpdt] Sending command to MQTT task.
53 29149 [MQTT] Received message 80000 from queue.
54 29277 [MQTT] MQTT Subscribe was accepted. Subscribed.
55 29277 [MQTT] Notifying task.
56 29281 [ShDemoUpdt] Command sent to MQTT task passed.
57 29283 [ShDemoUpdt] [Shadow 0] MQTT: Subscribe to rejected topic succeeded.
58 29285 [ShDemoUpdt] Sending command to MQTT task.
59 29286 [MQTT] Received message 90000 from queue.
60 29307 [MQTT] Notifying task.
61 29310 [ShDemoUpdt] Command sent to MQTT task passed.
62 29312 [ShDemoUpdt] [Shadow 0] MQTT: Publish to operation topic succeeded.
63 29555 [ShDemoUpdt] Successfully performed update.
64 29557 [Shd-IOT-0] Shd-IOT-0 changing desired state.
65 29560 [ShDemoUpdt] Performing Thing Shadow update.
66 29562 [ShDemoUpdt] Sending command to MQTT task.
67 29563 [MQTT] Received message a0000 from queue.
68 29583 [MQTT] Notifying task.
69 29586 [ShDemoUpdt] Command sent to MQTT task passed.
70 29588 [ShDemoUpdt] [Shadow 0] MQTT: Publish to operation topic succeeded.
71 29830 [ShDemoUpdt] Successfully performed update.
72 29832 [Shd-IOT-0] Shd-IOT-0 done changing desired state.
73 34557 [Shd-IOT-0] Shd-IOT-0 changing desired state.
74 34560 [ShDemoUpdt] Performing Thing Shadow update.
75 34562 [ShDemoUpdt] Sending command to MQTT task.
76 34563 [MQTT] Received message b0000 from queue.
77 34584 [MQTT] Notifying task.
78 34588 [ShDemoUpdt] Command sent to MQTT task passed.
79 34590 [ShDemoUpdt] [Shadow 0] MQTT: Publish to operation topic succeeded.
80 34850 [MQTT] Successfully added new reported state to update queue.
81 34868 [ShDemoUpdt] Successfully performed update.
82 34870 [ShDemoUpdt] Performing Thing Shadow update.
83 34872 [Shd-IOT-0] Shd-IOT-0 done changing desired state.
84 34873 [ShDemoUpdt] Sending command to MQTT task.
85 34875 [MQTT] Received message c0000 from queue.
86 34894 [MQTT] Notifying task.
87 34898 [ShDemoUpdt] Command sent to MQTT task passed.
88 34900 [ShDemoUpdt] [Shadow 0] MQTT: Publish to operation topic succeeded.
89 35142 [ShDemoUpdt] Successfully performed update.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

