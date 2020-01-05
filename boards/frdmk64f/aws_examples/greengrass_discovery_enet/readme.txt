Overview
========
This example demonstrates how the board can discover Greengrass core and communicate with AWS IoT cloud through it.
You will need device (A Mac, Windows PC, or UNIX-like system) for running AWS Greengrass. Example will connect to WiFi network, try to discover your AWS Greengrass device and send Hello World message to AWS IoT cloud through it.
This demo needs network with internet access and opened 8883 and 8443 ports.


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

2.  Open example's project and build it.

3.  Connect a USB cable between the PC host and the OpenSDA port on the target board.

4.  Open a serial terminal on PC for serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control

5.  Connect the boards's RJ45 to network with Internet access (IP address to the board is assigned by the DHCP server). Make sure the connection on port 8883 is not blocked.

6.  Download the program to the target board.

7.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================

Demo is by default set to send message "Hello #0 from Amazon FreeRTOS to Greengrass Core." 3 times (can be changed with ggdDEMO_MAX_MQTT_MESSAGES).

You can check connection log in Greengrass device on path: /greengrass/ggc/var/log/system/connection_manager.log


~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Initializing PHY...
0 159 [Tmr Svc] Write certificate...
1 468 [Tmr Svc] Device credential provisioning succeeded.
2 2091 [Tmr Svc] Getting IP address from DHCP ...
3 5091 [Tmr Svc] IPv4 Address: 192.168.2.102
4 5091 [Tmr Svc] DHCP OK
5 5094 [iot_thread] [INFO ][INIT][5094] SDK successfully initialized.
6 5094 [iot_thread] [INFO ][DEMO][5094] Successfully initialized the demo. Network type for the demo: 4
7 5094 [iot_thread] Attempting automated selection of Greengrass device
8 16847 [iot_thread] About to close socket.
9 16849 [iot_thread] Socket closed.
10 16849 [iot_thread] Stack high watermark for discovery helper task: 1409.
11 21021 [iot_thread] About to close socket.
12 21024 [iot_thread] Socket closed.
13 21024 [iot_thread] Stack high watermark for discovery helper task: 759.
14 21024 [iot_thread] Greengrass device discovered.
15 21024 [iot_thread] Establishing MQTT communication to Greengrass...
16 25201 [iot_thread] [INFO ][MQTT][25201] Establishing new MQTT connection.
17 25203 [iot_thread] [INFO ][MQTT][25203] Anonymous metrics (SDK language, SDK version) will be provided to AWS IoT. Recompile with AWS_IOT_MQTT_ENABLE_METRICS set to 0 to disable.
18 25204 [iot_thread] [INFO ][MQTT][25204] (MQTT connection 2000c520, CONNECT operation 20006448) Waiting for operation completion.
19 25207 [iot_thread] [INFO ][MQTT][25207] (MQTT connection 2000c520, CONNECT operation 20006448) Wait complete with result SUCCESS.
20 25208 [iot_thread] [INFO ][MQTT][25208] New MQTT connection 2000b2a8 established.
21 25209 [iot_thread] [INFO ][MQTT][25209] (MQTT connection 2000c520) MQTT PUBLISH operation queued.
22 26709 [iot_thread] [INFO ][MQTT][26709] (MQTT connection 2000c520) MQTT PUBLISH operation queued.
23 28209 [iot_thread] [INFO ][MQTT][28209] (MQTT connection 2000c520) MQTT PUBLISH operation queued.
24 29709 [iot_thread] Disconnecting from broker.
25 29709 [iot_thread] [INFO ][MQTT][29709] (MQTT connection 2000c520) Disconnecting connection.
26 29709 [iot_thread] [INFO ][MQTT][29709] (MQTT connection 2000c520, DISCONNECT operation 2000c638) Waiting for operation completion.
27 29710 [iot_thread] [INFO ][MQTT][29709] (MQTT connection 2000c520, DISCONNECT operation 2000c638) Wait complete with result SUCCESS.
28 29711 [iot_thread] [INFO ][MQTT][29711] (MQTT connection 2000c520) Connection disconnected.
29 29712 [iot_thread] [INFO ][MQTT][29712] (MQTT connection 2000c520) Network connection closed.
30 29712 [iot_thread] [INFO ][MQTT][29712] (MQTT connection 2000c520) Network connection destroyed.
31 29712 [iot_thread] Disconnected from the broker.
32 29712 [iot_thread] Deleted Client.
33 29714 [iot_thread] Heap low watermark: 11904. Stack high watermark: 759.
34 29714 [iot_thread] ----Demo finished----
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

