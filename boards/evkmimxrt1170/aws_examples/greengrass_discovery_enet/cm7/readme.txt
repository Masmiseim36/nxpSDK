Overview
========
This example demonstrates how the board can discover Greengrass core and communicate with AWS IoT cloud through it.
You will need device (A Mac, Windows PC, or UNIX-like system) for running AWS Greengrass. Example try to discover your AWS Greengrass device and send Hello World message to AWS IoT cloud through it.
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
- IAR embedded Workbench  9.20.2
- Keil MDK  5.36
- GCC ARM Embedded  10.3.1
- MCUXpresso  11.5.0

Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT1170-EVK board
- Personal Computer
- Network cable RJ45 standard (Network with Internet access)

Board settings
==============
This example uses 1G port(J4) as default. If want to test 100M port(J3), please set the macro BOARD_NETWORK_USE_100M_ENET_PORT to 1.

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

Demo is by default set to send message "Hello #0 from FreeRTOS to Greengrass Core." 3 times (can be changed with ggdDEMO_MAX_MQTT_MESSAGES).

You can check connection log in Greengrass device on path: /greengrass/ggc/var/log/system/connection_manager.log


~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Initializing PHY...
0 172 [Tmr Svc] Write certificate...

1 1815 [Tmr Svc] Getting IP address from DHCP ...

2 2815 [Tmr Svc] IPv4 Address: 192.168.199.100

3 2815 [Tmr Svc] DHCP OK

4 2816 [iot_thread] [INFO ][DEMO][2816] ---------STARTING DEMO---------


5 2818 [iot_thread] [INFO ][INIT][2818] SDK successfully initialized.

6 2818 [iot_thread] [INFO ][DEMO][2818] Successfully initialized the demo. Network type for the demo: 4

7 2818 [iot_thread] [INFO ][MQTT][2818] MQTT library successfully initialized.

8 2818 [iot_thread] [INFO ][DEMO][2818] Attempting automated selection of Greengrass device


9 4505 [iot_thread] JSON file retrieval completed

10 4505 [iot_thread] About to close socket.

11 4506 [iot_thread] Socket closed.

12 4506 [iot_thread] Stack high watermark for discovery helper task: 1462.

13 5215 [iot_thread] About to close socket.

14 5217 [iot_thread] Socket closed.

15 5217 [iot_thread] Stack high watermark for discovery helper task: 814.

16 5217 [iot_thread] [INFO ][DEMO][5217] Greengrass device discovered.

17 5217 [iot_thread] [INFO ][DEMO][5217] Attempting to establish MQTT connection to Greengrass.

18 5935 [iot_thread] [INFO ][MQTT][5935] Establishing new MQTT connection.

19 5937 [iot_thread] [INFO ][MQTT][5937] (MQTT connection 202d1828, CONNECT operation 202d18e8) Waiting for operation completion.

20 6147 [NetRecv] [INFO] CONNACK session present bit not set.
21 6147 [NetRecv] [INFO] Connection accepted.
22 6148 [iot_thread] [INFO ][MQTT][6148] (MQTT connection 202d1828, CONNECT operation 202d18e8) Wait complete with result SUCCESS.

23 6149 [iot_thread] [INFO ][MQTT][6149] New MQTT connection 202ce860 established.

24 6150 [iot_thread] [INFO ][MQTT][6150] (MQTT connection 202d1828) MQTT PUBLISH operation queued.

25 7651 [iot_thread] [INFO ][MQTT][7651] (MQTT connection 202d1828) MQTT PUBLISH operation queued.

26 9152 [iot_thread] [INFO ][MQTT][9152] (MQTT connection 202d1828) MQTT PUBLISH operation queued.

27 10652 [iot_thread] [INFO ][DEMO][10652] Disconnecting from broker.

28 10652 [iot_thread] [INFO ][MQTT][10652] (MQTT connection 202d1828) Disconnecting connection.

29 10653 [iot_thread] [INFO] Disconnected from the broker.
30 10653 [iot_thread] [INFO ][MQTT][10653] (MQTT connection 202d1828) Network connection closed.

31 10905 [iot_thread] [INFO ][MQTT][10905] (MQTT connection 202d1828) Network connection destroyed.

32 10905 [iot_thread] [INFO ][DEMO][10905] Disconnected from the broker.

33 10905 [iot_thread] Heap low watermark: 21784. Stack high watermark: 814.

34 10905 [iot_thread] [INFO ][MQTT][10905] MQTT library cleanup done.

35 10905 [iot_thread] [INFO ][DEMO][10905] Cleaned up MQTT library.

36 10905 [iot_thread] [INFO ][DEMO][10905] memory_metrics::freertos_heap::before::bytes::72392

37 10905 [iot_thread] [INFO ][DEMO][10905] memory_metrics::freertos_heap::after::bytes::21784

38 10905 [iot_thread] [INFO ][DEMO][10905] memory_metrics::demo_task_stack::before::bytes::7856

39 10905 [iot_thread] [INFO ][DEMO][10905] memory_metrics::demo_task_stack::after::bytes::3256

40 11905 [iot_thread] [INFO ][DEMO][11905] Demo completed successfully.

41 11906 [iot_thread] [INFO ][INIT][11906] SDK cleanup done.

42 11906 [iot_thread] [INFO ][DEMO][11906] -------DEMO FINISHED----
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
