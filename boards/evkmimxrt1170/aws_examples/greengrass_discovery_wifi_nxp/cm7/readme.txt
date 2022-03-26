Overview
========
This example demonstrates how the board can discover Greengrass core and communicate with AWS IoT cloud through it.
You will need device (A Mac, Windows PC, or UNIX-like system) for running AWS Greengrass. Example will connect to Wi-Fi network, try to discover your AWS Greengrass device and send Hello World message to AWS IoT cloud through it.
This demo needs Wi-Fi network with internet access and opened 8883 and 8443 ports.

Before building the example application select Wi-Fi module macro in the app_config.h. (see #define WIFI_<SoC Name>_BOARD_<Module Name>).
For more information about Wi-Fi module connection see:
    readme_modules.txt
    Getting started guide on supported modules configuration:
    https://www.nxp.com/document/guide/getting-started-with-nxp-wi-fi-modules-using-i-mx-rt-platform:GS-WIFI-MODULES-IMXRT-PLATFORM


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
- IAR embedded Workbench  9.20.2
- Keil MDK  5.36
- GCC ARM Embedded  10.3.1
- MCUXpresso  11.5.0

Hardware requirements
=====================
- Micro USB cable
- MIMXRT1170-EVK board
- Personal Computer


Board settings
==============

Prepare the Demo
================
1.  Open file "aws_clientcredential.h" and fill in your "Thing name" and "REST API endpoint" which can be found in AWS IoT under "Greengrass/Devices/<your device>/Interact":

    Example:
        static const char clientcredentialMQTT_BROKER_ENDPOINT[] = "abcdefgh123456.iot.us-west-2.amazonaws.com";
        #define clientcredentialIOT_THING_NAME "HelloWorldDevice"

    The device certificate and private key needs to be opened in text editor and its content copied into the "aws_clientcredential_keys.h" or you can use the CertificateConfigurator.html (mcu-sdk-2.0\rtos\freertos\tools\certificate_configuration) to generate the "aws_clientcredential_keys.h".

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

2.  Update these macros in "aws_clientcredential.h" based on your Wi-Fi network configuration:
        #define clientcredentialWIFI_SSID       "Paste Wi-Fi SSID here."
        #define clientcredentialWIFI_PASSWORD   "Paste Wi-Fi password here."

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
0 128 [Tmr Svc] Write certificate...

1 154 [iot_thread] [INFO ][DEMO][154] ---------STARTING DEMO---------


2 156 [iot_thread] [INFO ][INIT][156] SDK successfully initialized.

MAC Address:  0:13:43:7F:9E:77 
[net] Initialized TCP/IP networking stack

3 3460 [iot_thread] Connecting to nxp .....

4 37015 [wlcmgr] Connected to with IP = [192.168.199.188]

5 37060 [iot_thread] [INFO ][DEMO][37060] Successfully initialized the demo. Network type for the demo: 1

6 37060 [iot_thread] [INFO ][MQTT][37060] MQTT library successfully initialized.

7 37060 [iot_thread] [INFO ][DEMO][37060] Attempting automated selection of Greengrass device


8 40491 [iot_thread] JSON file retrieval completed

9 40491 [iot_thread] About to close socket.

10 40494 [iot_thread] Socket closed.

11 40494 [iot_thread] Stack high watermark for discovery helper task: 1530.

12 41456 [iot_thread] About to close socket.

13 41457 [iot_thread] Socket closed.

14 41460 [iot_thread] Stack high watermark for discovery helper task: 882.

15 41460 [iot_thread] [INFO ][DEMO][41460] Greengrass device discovered.

16 41460 [iot_thread] [INFO ][DEMO][41460] Attempting to establish MQTT connection to Greengrass.

17 42487 [iot_thread] [INFO ][MQTT][42487] Establishing new MQTT connection.

18 42491 [iot_thread] [INFO ][MQTT][42491] (MQTT connection 202144f0, CONNECT operation 202145b0) Waiting for operation completion.

19 42736 [NetRecv] [INFO] CONNACK session present bit not set.
20 42736 [NetRecv] [INFO] Connection accepted.
21 42737 [iot_thread] [INFO ][MQTT][42737] (MQTT connection 202144f0, CONNECT operation 202145b0) Wait complete with result SUCCESS.

22 42738 [iot_thread] [INFO ][MQTT][42738] New MQTT connection 2020bf20 established.

23 42740 [iot_thread] [INFO ][MQTT][42740] (MQTT connection 202144f0) MQTT PUBLISH operation queued.

24 44241 [iot_thread] [INFO ][MQTT][44241] (MQTT connection 202144f0) MQTT PUBLISH operation queued.

25 45742 [iot_thread] [INFO ][MQTT][45741] (MQTT connection 202144f0) MQTT PUBLISH operation queued.

26 47242 [iot_thread] [INFO ][DEMO][47242] Disconnecting from broker.

27 47242 [iot_thread] [INFO ][MQTT][47242] (MQTT connection 202144f0) Disconnecting connection.

28 47242 [iot_thread] [INFO] Disconnected from the broker.
29 47244 [iot_thread] [INFO ][MQTT][47244] (MQTT connection 202144f0) Network connection closed.

30 47477 [iot_thread] [INFO ][MQTT][47477] (MQTT connection 202144f0) Network connection destroyed.

31 47477 [iot_thread] [INFO ][DEMO][47477] Disconnected from the broker.

32 47477 [iot_thread] Heap low watermark: 14984. Stack high watermark: 882.

33 47477 [iot_thread] [INFO ][MQTT][47477] MQTT library cleanup done.

34 47477 [iot_thread] [INFO ][DEMO][47477] Cleaned up MQTT library.

35 47479 [iot_thread] [INFO ][DEMO][47479] memory_metrics::freertos_heap::before::bytes::87384

36 47479 [iot_thread] [INFO ][DEMO][47479] memory_metrics::freertos_heap::after::bytes::14984

37 47479 [iot_thread] [INFO ][DEMO][47479] memory_metrics::demo_task_stack::before::bytes::7856

38 47479 [iot_thread] [INFO ][DEMO][47479] memory_metrics::demo_task_stack::after::bytes::3528

39 48479 [iot_thread] [INFO ][DEMO][48479] Demo completed successfully.

40 48484 [wlcmgr] Dis-connected

41 48529 [iot_thread] [INFO ][INIT][48529] SDK cleanup done.

42 48529 [iot_thread] [INFO ][DEMO][48529] -------DEMO FINISHED-------

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
