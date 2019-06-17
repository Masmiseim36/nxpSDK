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
- IAR embedded Workbench  8.32.3
- GCC ARM Embedded  8.2.1
- MCUXpresso  11.0.0

Hardware requirements
=====================
- Micro USB cable
- evkmimxrt1060 board
- Personal Computer
- Murata 1DX M.2 module
- Murata uSD M.2 Adapter

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
0 0 [Tmr Svc] Starting key provisioning...
1 0 [Tmr Svc] Write root certificate...
2 14 [Tmr Svc] Write device private key...
3 488 [Tmr Svc] Write device certificate...
4 506 [Tmr Svc] Key provisioning done...
5 507 [Tmr Svc] Starting WiFi...
6 2075 [Tmr Svc] WiFi module initialized.
7 10233 [Tmr Svc] WiFi connected to AP RedmiAWS.
8 10234 [Tmr Svc] IP Address acquired 192.168.43.251
9 10242 [IoT_GGD] Attempting automated selection of Greengrass device
11 28394 [IoT_GGD] About to close socket.
12 28437 [IoT_GGD] Socket closed.
13 28440 [IoT_GGD] Stack high watermark for discovery helper task: 1556.
14 28479 [IoT_GGD] Looked up 192.168.43.248 as 192.168.43.248
15 39777 [IoT_GGD] About to close socket.
16 39826 [IoT_GGD] Socket closed.
17 39828 [IoT_GGD] Stack high watermark for discovery helper task: 898.
18 39829 [IoT_GGD] Greengrass device discovered.
19 39831 [IoT_GGD] Establishing MQTT communication to Greengrass...
20 39833 [MQTT] Looked up 192.168.43.248 as 192.168.43.248
21 55890 [IoT_GGD] Disconnecting from broker.
22 55965 [IoT_GGD] Disconnected from the broker.
23 55966 [IoT_GGD] Deleted Client.
24 55970 [IoT_GGD] Heap low watermark: 3632. Stack high watermark: 898.
25 55972 [IoT_GGD] ----Demo finished----
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

