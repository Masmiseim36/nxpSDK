Overview
========
This example demonstrates how the board can discover Greengrass core and communicate with AWS IoT cloud through it.
You will need device (A Mac, Windows PC, or UNIX-like system) for running AWS Greengrass. Example will connect to WiFi network, try to discover your AWS Greengrass device and send Hello World message to AWS IoT cloud through it.
This demo needs WiFi network with internet access and opened 8883 and 8443 ports.


Prepare the AWS Greengrass and AWS IoT
Please update WiFi firmware to version 3.3.6 using "qca_fwupdate" demo.
Before running the demo it is needed to configure AWS IoT Console, AWS Greengrass:

1.  Create AWS Account: https://console.aws.amazon.com/console/home

2.  Create AWS Greengrass group, set up it's core and set up Hello World Lambda function (Module 1 to Module 3 Part 1): https://docs.aws.amazon.com/greengrass/latest/developerguide/gg-gs.html

    Note: If you are using Amazon EC2 service for running Greengrass Core, make sure that its public IP address (you can see when selecting your instance in AWS EC2 service administration) is set in Greengrass Core/Connectivity.

3.  Set up your device as part of your Greengrass group: https://docs.aws.amazon.com/greengrass/latest/developerguide/device-group.html

    In 2. step create one device named for example "HelloWorldDevice" (this will be your "Thing name") and save device certificates, you will need them later.

    Make note of "REST API endpoint" in Greengrass/Devices/<your device>/Interact

4.  Make sure that Greengrass core is running (e.g. on Linux you can check it with "ps aux | grep -e "greengrass"" command). If it is not running, start it with "sudo ./greengrassd start" in folder "/greengrass/ggc/packages/x.x.x/".

5.  Go to AWS IoT, find your Greengrass group and deploy it again. You should do new deployment after every configuration change.



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

1.  Open file "aws_clientcredential.h" and fill in your "Thing name" and "REST API endpoint" which can be found in AWS IoT under "Greengrass/Devices/<your device>/Interact":

    Example:
        static const char clientcredentialMQTT_BROKER_ENDPOINT[] = "abcdefgh123456.iot.us-west-2.amazonaws.com";
        #define clientcredentialIOT_THING_NAME "HelloWorldDevice"

    The device certificate and private key needs to be opened in text editor and its content copied into the "aws_clientcredential_keys.h" or you can use the CertificateConfigurator.html (mcu-sdk-2.0\rtos\amazon-freertos\tools\certificate_configuration) to generate the "aws_clientcredential_keys.h".

    keyCLIENT_CERTIFICATE_PEM is stored in <device id>.cert.pem file and keyCLIENT_PRIVATE_KEY_PEM is stored in <device id>.private.key file.

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

2.  Update these macros in "aws_clientcredential.h" based on your WiFi network configuration:
        #define clientcredentialWIFI_SSID       "Paste WiFi SSID here."
        #define clientcredentialWIFI_PASSWORD   "Paste WiFi password here."

3.  Open example's project and build it.

4.  Connect a USB cable between the PC host and the OpenSDA port on the target board.

5.  Open a serial terminal on PC for serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control

6.  Download the program to the target board.

7.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================

Demo is by default set to send message "Hello #0 from Amazon FreeRTOS to Greengrass Core." 3 times (can be changed with ggdDEMO_MAX_MQTT_MESSAGES).

You can check connection log in Greengrass device on path: /greengrass/ggc/var/log/system/connection_manager.log


~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
0 160 [Tmr Svc] Write certificate...
1 470 [Tmr Svc] Device credential provisioning succeeded.
2 473 [iot_thread] [INFO ][INIT][473] SDK successfully initialized.
3 2343 [iot_thread] [INFO ][DEMO][2343] Successfully initialized the demo. Network type for the demo: 1
4 2343 [iot_thread] Attempting automated selection of Greengrass device
6 7785 [iot_thread] About to close socket.
7 7789 [iot_thread] Socket closed.
8 7789 [iot_thread] Stack high watermark for discovery helper task: 1411.
9 7793 [iot_thread] Looked up 10.42.0.1 as 10.42.0.1
10 12066 [iot_thread] About to close socket.
11 12069 [iot_thread] Socket closed.
12 12069 [iot_thread] Stack high watermark for discovery helper task: 761.
13 12069 [iot_thread] Greengrass device discovered.
14 12069 [iot_thread] Establishing MQTT communication to Greengrass...
15 12071 [iot_thread] Looked up 10.42.0.1 as 10.42.0.1
16 16243 [iot_thread] [INFO ][MQTT][16243] Establishing new MQTT connection.
17 16245 [iot_thread] [INFO ][MQTT][16245] Anonymous metrics (SDK language, SDK version) will be provided to AWS IoT. Recompile with AWS_IOT_MQTT_ENABLE_METRICS set to 0 to disable.
18 16246 [iot_thread] [INFO ][MQTT][16246] (MQTT connection 20009c48, CONNECT operation 20009d60) Waiting for operation completion.
19 16252 [iot_thread] [INFO ][MQTT][16252] (MQTT connection 20009c48, CONNECT operation 20009d60) Wait complete with result SUCCESS.
20 16253 [iot_thread] [INFO ][MQTT][16253] New MQTT connection 20009760 established.
21 16255 [iot_thread] [INFO ][MQTT][16253] (MQTT connection 20009c48) MQTT PUBLISH operation queued.
22 17755 [iot_thread] [INFO ][MQTT][17755] (MQTT connection 20009c48) MQTT PUBLISH operation queued.
23 19255 [iot_thread] [INFO ][MQTT][19255] (MQTT connection 20009c48) MQTT PUBLISH operation queued.
24 20755 [iot_thread] Disconnecting from broker.
25 20755 [iot_thread] [INFO ][MQTT][20755] (MQTT connection 20009c48) Disconnecting connection.
26 20756 [iot_thread] [INFO ][MQTT][20755] (MQTT connection 20009c48, DISCONNECT operation 20009d60) Waiting for operation completion.
27 20756 [iot_thread] [INFO ][MQTT][20756] (MQTT connection 20009c48, DISCONNECT operation 20009d60) Wait complete with result SUCCESS.
28 20756 [iot_thread] [INFO ][MQTT][20756] (MQTT connection 20009c48) Connection disconnected.
29 20756 [iot_thread] [INFO ][MQTT][20756] (MQTT connection 20009c48) Network connection closed.
30 20764 [iot_thread] [INFO ][MQTT][20764] (MQTT connection 20009c48) Network connection destroyed.
31 20764 [iot_thread] Disconnected from the broker.
32 20764 [iot_thread] Deleted Client.
33 20764 [iot_thread] Heap low watermark: 4136. Stack high watermark: 761.
34 20764 [iot_thread] ----Demo finished----
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

