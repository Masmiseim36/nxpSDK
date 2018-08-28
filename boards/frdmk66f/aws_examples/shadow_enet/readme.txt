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

4.  Connect a USB cable between the PC host and the OpenSDA port on the target board.

5.  Open a serial terminal on PC for serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control

6.  Connect the boards's RJ45 to network with Internet access (IP address to the board is assigned by the DHCP server). Make sure the connection on port 8883 is not blocked.

7.  Download the program to the target board.

8.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
The log below shows the output of the demo in the terminal window.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
0 0 [Tmr Svc] Starting key provisioning...
1 0 [Tmr Svc] Write root certificate...
2 0 [Tmr Svc] Write device private key...
3 0 [Tmr Svc] Key provisioning done...
4 6249 [Tmr Svc] Getting IP address from DHCP ...
5 7249 [Tmr Svc] IPv4 Address: 10.42.0.198
6 7249 [Tmr Svc] DHCP OK
7 7255 [MainDemoTask] [Shadow 0] MQTT: Creation of dedicated MQTT client succeeded.
8 7262 [MainDemoTask] Sending command to MQTT task.
9 7267 [MQTT] Received message 10000 from queue.
10 15949 [MQTT] MQTT Connect was accepted. Connection established.
11 15949 [MQTT] Notifying task.
12 15957 [MainDemoTask] Command sent to MQTT task passed.
13 15963 [MainDemoTask] [Shadow 0] MQTT: Connect succeeded.
14 15968 [MainDemoTask] Sending command to MQTT task.
15 15973 [MQTT] Received message 20000 from queue.
16 16378 [MQTT] MQTT Subscribe was accepted. Subscribed.
17 16378 [MQTT] Notifying task.
18 16385 [MainDemoTask] Command sent to MQTT task passed.
19 16391 [MainDemoTask] [Shadow 0] MQTT: Subscribe to callback topic succeeded.
20 16398 [MainDemoTask] Sending command to MQTT task.
21 16402 [MQTT] Received message 30000 from queue.
22 16707 [MQTT] MQTT Subscribe was accepted. Subscribed.
23 16707 [MQTT] Notifying task.
24 16715 [MainDemoTask] Command sent to MQTT task passed.
25 16720 [MainDemoTask] [Shadow 0] MQTT: Subscribe to callback topic succeeded.
26 16727 [MainDemoTask] Sending command to MQTT task.
27 16731 [MQTT] Received message 40000 from queue.
28 17036 [MQTT] MQTT Subscribe was accepted. Subscribed.
29 17036 [MQTT] Notifying task.
30 17044 [MainDemoTask] Command sent to MQTT task passed.
31 17049 [MainDemoTask] [Shadow 0] MQTT: Subscribe to callback topic succeeded.
32 17056 [MainDemoTask] Sending command to MQTT task.
33 17060 [MQTT] Received message 50000 from queue.
34 17365 [MQTT] MQTT Subscribe was accepted. Subscribed.
35 17365 [MQTT] Notifying task.
36 17373 [MainDemoTask] Command sent to MQTT task passed.
37 17378 [MainDemoTask] [Shadow 0] MQTT: Subscribe to accepted topic succeeded.
38 17385 [MainDemoTask] Sending command to MQTT task.
39 17389 [MQTT] Received message 60000 from queue.
40 17795 [MQTT] MQTT Subscribe was accepted. Subscribed.
41 17795 [MQTT] Notifying task.
42 17803 [MainDemoTask] Command sent to MQTT task passed.
43 17808 [MainDemoTask] [Shadow 0] MQTT: Subscribe to rejected topic succeeded.
44 17815 [MainDemoTask] Sending command to MQTT task.
45 17819 [MQTT] Received message 70000 from queue.
46 17820 [MQTT] Notifying task.
47 17827 [MainDemoTask] Command sent to MQTT task passed.
48 17832 [MainDemoTask] [Shadow 0] MQTT: Publish to operation topic succeeded.
49 18225 [Shd-IOT-0] Sending command to MQTT task.
50 18226 [MQTT] Received message 80000 from queue.
51 19540 [MQTT] MQTT Subscribe was accepted. Subscribed.
52 19540 [MQTT] Notifying task.
53 19541 [Shd-IOT-0] Command sent to MQTT task passed.
54 19541 [Shd-IOT-0] [Shadow 0] MQTT: Subscribe to accepted topic succeeded.
55 19541 [Shd-IOT-0] Sending command to MQTT task.
56 19541 [MQTT] Received message 90000 from queue.
57 20451 [MQTT] MQTT Subscribe was accepted. Subscribed.
58 20451 [MQTT] Notifying task.
59 20452 [Shd-IOT-0] Command sent to MQTT task passed.
60 20452 [Shd-IOT-0] [Shadow 0] MQTT: Subscribe to rejected topic succeeded.
61 20452 [Shd-IOT-0] Sending command to MQTT task.
62 20452 [MQTT] Received message a0000 from queue.
63 20452 [MQTT] Notifying task.
64 20453 [Shd-IOT-0] Command sent to MQTT task passed.
65 20453 [Shd-IOT-0] [Shadow 0] MQTT: Publish to operation topic succeeded.
66 21463 [Shd-IOT-0] Shadow Demo initialized.
67 21463 [Shd-IOT-0] Client request to change color of light bulb.
68 21463 [Shd-IOT-0] Sending command to MQTT task.
69 21464 [MQTT] Received message b0000 from queue.
70 21464 [MQTT] Notifying task.
71 21464 [Shd-IOT-0] Command sent to MQTT task passed.
72 21464 [Shd-IOT-0] [Shadow 0] MQTT: Publish to operation topic succeeded.
73 23285 [ShDemoUpdt] Performing thing update.
74 23387 [Shd-IOT-0] Client change done in thing shadow.
75 23392 [ShDemoUpdt] Sending command to MQTT task.
76 23396 [MQTT] Received message c0000 from queue.
77 23397 [MQTT] Notifying task.
78 23404 [ShDemoUpdt] Command sent to MQTT task passed.
79 23409 [ShDemoUpdt] [Shadow 0] MQTT: Publish to operation topic succeeded.
80 25117 [ShDemoUpdt] Performing thing update complete.
.
.
.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

