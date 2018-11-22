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
- Keil MDK 5.25
- IAR embedded Workbench 8.30.1
- GCC ARM Embedded 7-2017-q4-major
- MCUXpresso10.2.1

Hardware requirements
=====================

Board settings
==============
Plug SX-ULPAN-2401-SHIELD(US) board to J22, J23, J24, J25.
Dis-connect J15, and weld 0Ω resistor to R278,R279,R280,R281.

Please use external power supply (J2), USB may not be sufficient.

Prepare the Demo
================

1.  Open file "aws_clientcredential.h" and fill in your "Thing name" and "REST API endpoint" which can be found in AWS IoT under "Greengrass/Devices/<your device>/Interact":

    Example:
        static const char clientcredentialMQTT_BROKER_ENDPOINT[] = "abcdefgh123456.iot.us-west-2.amazonaws.com";
        #define clientcredentialIOT_THING_NAME "HelloWorldDevice"

    Each of device certificates needs to be opened in text editor and its content copied into the "aws_clientcredential_keys.h" or you can use the CertificateConfigurator.html (mcu-sdk-2.0\rtos\amazon-freertos\demos\common\devmode_key_provisioning\CertificateConfigurationTool) to generate the "aws_clientcredential_keys.h".

    clientcredentialCLIENT_CERTIFICATE_PEM is stored in <device id>.cert.pem file and clientcredentialCLIENT_PRIVATE_KEY_PEM is stored in <device id>.private.key file.

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

2.  Update these macros in "aws_clientcredential.h" based on your WiFi network configuration:
        #define clientcredentialWIFI_SSID       "Paste WiFi SSID here."
        #define clientcredentialWIFI_PASSWORD   "Paste WiFi password here."

3.  Open example's project and build it.

4.  Connect a USB cable between the PC host and the OpenSDA port on the target board.

5.  Download the program to the target board with a J-Link debugger.

6.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

7.  Open a serial terminal on PC for serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control

Running the demo
================

Demo is by default set to send message "Hello #1 from device to Greengrass Core." 5 times (can be changed with ggdDEMO_MAX_MQTT_MESSAGES).

You can check connection log in Greengrass device on path: /greengrass/ggc/var/log/system/connection_manager.log


~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
0 0 [Tmr Svc] Starting key provisioning...

1 0 [Tmr Svc] Write root certificate...

2 0 [Tmr Svc] Write device private key...

3 9 [Tmr Svc] Write device certificate...

4 18 [Tmr Svc] Key provisioning done...

5 18 [Tmr Svc] Starting WiFi...

6 1266 [Tmr Svc] WiFi module initialized.

7 5274 [Tmr Svc] WiFi connected to AP AWS_WiFi.

8 9278 [Tmr Svc] IP Address acquired 192.168.1.10

9 9278 [IoT_GGD] Attempting automated selection of Greengrass device

10 9954 [IoT_GGD] Looked up a254jqzk0kcvf0.iot.us-west-2.amazonaws.com as 52.34.174.227

11 22301 [IoT_GGD] About to close socket.

12 22306 [IoT_GGD] Socket closed.

13 22319 [IoT_GGD] Looked up 192.168.1.20 as 192.168.1.20

14 26478 [IoT_GGD] About to close socket.

15 26482 [IoT_GGD] Socket closed.

16 26482 [IoT_GGD] Greengrass device discovered.

17 26482 [IoT_GGD] Establishing MQTT communication to Greengrass...

18 26484 [IoT_GGD] Sending command to MQTT task.

19 26484 [MQTT] Received message 10000 from queue.

20 26495 [MQTT] Looked up 192.168.1.20 as 192.168.1.20

21 30786 [MQTT] MQTT Connect was accepted. Connection established.

22 30786 [MQTT] Notifying task.

23 30786 [IoT_GGD] Command sent to MQTT task passed.

24 30786 [IoT_GGD] Sending command to MQTT task.

25 30787 [MQTT] Received message 20000 from queue.

26 30788 [MQTT] Notifying task.

27 30788 [IoT_GGD] Command sent to MQTT task passed.

28 32288 [IoT_GGD] Sending command to MQTT task.

29 32288 [MQTT] Received message 30000 from queue.

30 32289 [MQTT] Notifying task.

31 32289 [IoT_GGD] Command sent to MQTT task passed.

32 33789 [IoT_GGD] Sending command to MQTT task.

33 33789 [MQTT] Received message 40000 from queue.

34 33790 [MQTT] Notifying task.

35 33790 [IoT_GGD] Command sent to MQTT task passed.

36 35290 [IoT_GGD] Sending command to MQTT task.

37 35290 [MQTT] Received message 50000 from queue.

38 35291 [MQTT] Notifying task.

39 35291 [IoT_GGD] Command sent to MQTT task passed.

40 36791 [IoT_GGD] Sending command to MQTT task.

41 36791 [MQTT] Received message 60000 from queue.

42 36792 [MQTT] Notifying task.

43 36792 [IoT_GGD] Command sent to MQTT task passed.

44 38292 [IoT_GGD] Disconnecting from broker.

45 38292 [IoT_GGD] Sending command to MQTT task.

46 38292 [MQTT] Received message 70000 from queue.

47 38293 [MQTT] About to close socket.

48 38298 [MQTT] Socket closed.

49 38298 [MQTT] Notifying task.

50 38299 [IoT_GGD] Command sent to MQTT task passed.

51 38299 [IoT_GGD] Disconnected from the broker.

52 38299 [IoT_GGD] Deleted Client.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

