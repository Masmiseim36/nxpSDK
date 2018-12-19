Overview
========
The simple Shadow lightbulb example to illustrate how client application and things communicate with the Shadow service.

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
2 15 [Tmr Svc] Write device private key...
3 477 [Tmr Svc] Write device certificate...
4 495 [Tmr Svc] Key provisioning done...
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
Hardware requirements
=====================
- Mini/micro USB cable
- EVK-MIMXRT1020 board
- Personal Computer
- Network cable RJ45 standard (Network with Internet access)

Board settings
==============
No special settings are required.


Note:
To debug in qspiflash, following steps are needed:
1. Select the flash target and compile.
3. Set the SW8: 1 off 2 off 3 on 4 off, then power on the board and connect USB cable to J23.
4. Start debugging in IDE.
   - Keil: Click "Download (F8)" to program the image to qspiflash first then clicking "Start/Stop Debug Session (Ctrl+F5)" to start debugging.

Toolchain supported
===================
- IAR embedded Workbench  8.32.1
- Keil MDK  5.26
- GCC ARM Embedded  7.3.1
- MCUXpresso 10.3.0

