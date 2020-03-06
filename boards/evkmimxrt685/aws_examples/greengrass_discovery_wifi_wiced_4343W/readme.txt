Overview
========
This example demonstrates how the board can discover Greengrass core and communicate with AWS IoT cloud through it.
You will need device (A Mac, Windows PC, or UNIX-like system) for running AWS Greengrass. Example will connect to WiFi network, try to discover your AWS Greengrass device and send Hello World message to AWS IoT cloud through it.
This demo needs WiFi network with internet access and opened 8883 and 8443 ports.


Prepare the AWS Greengrass and AWS IoT
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
- MCUXpresso  11.1.1

Hardware requirements
=====================
- Mini/micro USB cable
- EVK-MIMXRT685 board
- Murata 1DX M.2 module
- Murata uSD M.2 Adapter
- Personal Computer

Board settings
==============

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
0 120 [Tmr Svc] Write certificate...
1 249 [Tmr Svc] Device credential provisioning succeeded.
2 252 [iot_thread] [INFO ][INIT][252] SDK successfully initialized.

 AsyncInterrupt is not supported 
WLAN MAC Address : A0:C9:A0:3D:EE:8F
WLAN Firmware    : wl0: Feb 12 2018 04:08:14 version 7.79.2 (r683798 CY) FWID 01-27b63357
WLAN CLM         : API: 12.2 Data: 9.10.39 Compiler: 1.29.4 ClmImport: 1.36.3 Creation: 2018-02-12 04:00:50 
3 4953 [iot_thread] [INFO ][DEMO][4953] Successfully initialized the demo. Network type for the demo: 1
4 4953 [iot_thread] Attempting automated selection of Greengrass device
5 9194 [iot_thread] About to close socket.
6 9197 [iot_thread] Socket closed.
7 9197 [iot_thread] Stack high watermark for discovery helper task: 1536.
8 11717 [iot_thread] About to close socket.
9 11719 [iot_thread] Socket closed.
10 11719 [iot_thread] Stack high watermark for discovery helper task: 876.
11 11720 [iot_thread] Greengrass device discovered.
12 11720 [iot_thread] Establishing MQTT communication to Greengrass...
13 14204 [iot_thread] [INFO ][MQTT][14204] Establishing new MQTT connection.
14 14210 [iot_thread] [INFO ][MQTT][14210] Anonymous metrics (SDK language, SDK version) will be provided to AWS IoT. Recompile with AWS_IOT_MQTT_ENABLE_METRICS set to 0 to disable.
15 14211 [iot_thread] [INFO ][MQTT][14211] (MQTT connection 20209ee0, CONNECT operation 20209cf8) Waiting for operation completion.
16 14215 [iot_thread] [INFO ][MQTT][14215] (MQTT connection 20209ee0, CONNECT operation 20209cf8) Wait complete with result SUCCESS.
17 14216 [iot_thread] [INFO ][MQTT][14216] New MQTT connection 20209810 established.
18 14217 [iot_thread] [INFO ][MQTT][14217] (MQTT connection 20209ee0) MQTT PUBLISH operation queued.
19 15717 [iot_thread] [INFO ][MQTT][15717] (MQTT connection 20209ee0) MQTT PUBLISH operation queued.
20 17217 [iot_thread] [INFO ][MQTT][17217] (MQTT connection 20209ee0) MQTT PUBLISH operation queued.
21 18717 [iot_thread] Disconnecting from broker.
22 18717 [iot_thread] [INFO ][MQTT][18717] (MQTT connection 20209ee0) Disconnecting connection.
23 18717 [iot_thread] [INFO ][MQTT][18717] (MQTT connection 20209ee0, DISCONNECT operation 2020b158) Waiting for operation completion.
24 18719 [iot_thread] [INFO ][MQTT][18717] (MQTT connection 20209ee0, DISCONNECT operation 2020b158) Wait complete with result SUCCESS.
25 18719 [iot_thread] [INFO ][MQTT][18719] (MQTT connection 20209ee0) Connection disconnected.
26 18721 [iot_thread] [INFO ][MQTT][18721] (MQTT connection 20209ee0) Network connection closed.
27 18723 [iot_thread] [INFO ][MQTT][18723] (MQTT connection 20209ee0) Network connection destroyed.
28 18723 [iot_thread] Disconnected from the broker.
29 18723 [iot_thread] Deleted Client.
30 18723 [iot_thread] Heap low watermark: 2576. Stack high watermark: 876.
31 18723 [iot_thread] ----Demo finished----
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

