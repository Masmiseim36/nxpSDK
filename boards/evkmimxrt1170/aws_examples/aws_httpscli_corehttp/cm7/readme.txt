Overview
========
Simple https client example.


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
1.  Create AWS Account: https://console.aws.amazon.com/console/home

2.  Configure device in the AWS IoT Console base on this guide: https://docs.aws.amazon.com/iot/latest/developerguide/iot-sdk-setup.html

    Make note of example's "Thing name" and "REST API endpoint". These strings need to be set in the "aws_clientcredential.h".

    Example:
        static const char clientcredentialMQTT_BROKER_ENDPOINT[] = "abcdefgh123456.iot.us-west-2.amazonaws.com";
        #define clientcredentialIOT_THING_NAME "MyExample"

    In the next step you will get the "device certificate" and the "primary key". The device certificate and private key needs to be opened in text editor and its content copied into the "aws_clientcredential_keys.h".
    Or you can use the CertificateConfigurator.html (mcu-sdk-2.0\rtos\freertos\tools\certificate_configuration) to generate the "aws_clientcredential_keys.h".

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

3.  This demo does not need WiFi network with internet access. You can leave these defines empty.
    Macros in aws_clientcredential.h: 
        #define clientcredentialWIFI_SSID       ""
        #define clientcredentialWIFI_PASSWORD   ""

4.  Open example's project and build it.

5.  Connect a USB cable between the PC host and the OpenSDA port on the target board.

6.  Download the program to the target board with the CMSIS-DAP or J-Link debugger.

7.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

8.  Open a serial terminal on PC for serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control

Running the demo
================
The log below shows the output of the demo in the terminal window.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Initializing PHY...
0 122 [Tmr Svc] Write certificate...

1 1808 [Tmr Svc] Getting IP address from DHCP ...

2 2809 [Tmr Svc] IPv4 Address: 192.168.0.114

3 2809 [Tmr Svc] DHCP OK

4 2810 [iot_thread] [INFO ][DEMO][2810] ---------STARTING DEMO---------


5 2812 [iot_thread] [INFO ][INIT][2812] SDK successfully initialized.

6 2812 [iot_thread] [INFO ][DEMO][2812] Successfully initialized the demo. Network type for the demo: 4

7 2812 [iot_thread] [INFO] Establishing a TLS session to abcdefgh123456.iot.us-west-2.amazonaws.com:8443.
8 4475 [iot_thread] [INFO] Sending HTTP POST request to abcdefgh123456.iot.us-west-2.amazonaws.com/topics/topic?qos=1...
9 4907 [iot_thread] [INFO] Received HTTP response from abcdefgh123456.iot.us-west-2.amazonaws.com/topics/topic?qos=1...

10 4909 [iot_thread] [INFO] Demo iteration 1 was successful.
11 4909 [iot_thread] [INFO] Demo completed successfully.
12 4909 [iot_thread] [INFO ][DEMO][4909] memory_metrics::freertos_heap::before::bytes::74912

13 4909 [iot_thread] [INFO ][DEMO][4909] memory_metrics::freertos_heap::after::bytes::28696

14 4909 [iot_thread] [INFO ][DEMO][4909] memory_metrics::demo_task_stack::before::bytes::5360

15 4909 [iot_thread] [INFO ][DEMO][4909] memory_metrics::demo_task_stack::after::bytes::3640

16 5909 [iot_thread] [INFO ][DEMO][5909] Demo completed successfully.

17 5909 [iot_thread] [INFO ][INIT][5909] SDK cleanup done.

18 5909 [iot_thread] [INFO ][DEMO][5909] -------DEMO FINISHED-------
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
