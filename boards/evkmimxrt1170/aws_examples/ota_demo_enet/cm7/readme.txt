Overview
========
This example demonstrates how to perform OTA firmware update of the board using AWS IoT.

For Wi-Fi connection:
Before building the example application select Wi-Fi module macro in the app_config.h. (see #define WIFI_<SoC Name>_BOARD_<Module Name>).
For more information about Wi-Fi module connection see:
    readme_modules.txt
    Getting started guide on supported modules configuration:
    https://www.nxp.com/document/guide/getting-started-with-nxp-wi-fi-modules-using-i-mx-rt-platform:GS-WIFI-MODULES-IMXRT-PLATFORM


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
The demo requires MCUBoot booloader to be present in the FLASH memory to function properly.
It is recommended to build and program the bootloader first, then go on with the application.
Please refer to respective readme of the mcuboot_opensource example and follow the steps there before you continue.

Prior launching the demo it is recommended to pre-build image of modified version of the application to test the OTA update process.
For the new version to get recognized it is required to use a higher version number in xAppFirmwareVersion structure. This may be achieved e.g. by modifying the initialization macros used.

For comprehensive guide on seeting up AWS OTA, please see "AWS OTA User Guide.pdf" document.

Before running the demo it is mecessary to configure AWS IoT Console and update some of project files:

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

3.  In case your board connects to the Internet using WiFi, it is necessary to configure also WiFi parameters in "aws_clientcredential.h",
    namely "clientcredentialWIFI_SSID" and "clientcredentialWIFI_PASSWORD".
    Otherwise connect board's RJ45 to a network with Internet access.
    Either way the example expects IP configuration to be assigned by DHCP server.

4.  Open example's project and build it.
    Known issue: MDK linker issues warning about unused boot_hdr sections. This does not affect the functionality of the example.

5.  Connect a USB cable between the PC host and the OpenSDA USB port on the target board.

6.  Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control

Running the demo
================
Platform specific behavior:
When an application programmatically performs software-induced reset while in a debugging session, the execution stalls in the ROM code. This is a feature.
Your attention is needed at that moment: please perform reset manually by pressing the on-board hw reset button once you spot "SystemReset" message in the serial console.
Manual reset is not needed while MCU is running freely without a debugger.
