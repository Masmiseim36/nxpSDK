Overview
========
This example demonstrates how to perform OTA firmware update of the board using AWS IoT.

For Wi-Fi connection:
Before building the example application select Wi-Fi module macro in the app_config.h. (see #define WIFI_<SoC Name>_BOARD_<Module Name>).
For more information about Wi-Fi module connection see:
    readme_modules.txt
    Getting started guide on supported modules configuration:
    https://www.nxp.com/document/guide/getting-started-with-nxp-wi-fi-modules-using-i-mx-rt-platform:GS-WIFI-MODULES-IMXRT-PLATFORM



SDK version
===========
- Version: 2.16.000

Toolchain supported
===================
- IAR embedded Workbench  9.60.1
- Keil MDK  5.39.0
- GCC ARM Embedded  13.2.1
- MCUXpresso  11.10.0

Hardware requirements
=====================
- Micro USB cable
- MIMXRT1170-EVK board
- Personal Computer


Board settings
==============

Prepare the Demo
================
The demo requires MCUBoot bootloader to be present in the FLASH memory to function properly.
It is recommended to build and program the bootloader first, then go on with the application.
Please refer to respective readme of the mcuboot_opensource example and follow the steps there before you continue.

Prior launching the demo it is recommended to pre-build image of modified version of the application to test the OTA update process.
For the new version to get recognized it is required to use a higher version number in appFirmwareVersion structure. This may be achieved e.g. by modifying the initialization macros in ota_update.c:
    #define APP_VERSION_MAJOR 0
    #define APP_VERSION_MINOR 9
    #define APP_VERSION_BUILD 3


Creating an Application Code Signing Certificate:
In your working directory, copy the following text and create a file named cert_config.txt. In the text, replace test_signer@amazon.com with your email address.
------------------------------------
[ req ]
prompt             = no
distinguished_name = my_dn

[ my_dn ]
commonName = test_signer@amazon.com

[ my_exts ]
keyUsage         = digitalSignature
extendedKeyUsage = codeSigning
------------------------------------

Create an ECDSA code-signing private key:
    openssl genpkey -algorithm EC -pkeyopt ec_paramgen_curve:P-256 -pkeyopt ec_param_enc:named_curve -outform PEM -out ecdsasigner.key

Create an ECDSA code-signing certificate:
    openssl req -new -x509 -config cert_config.txt -extensions my_exts -nodes -days 365 -key ecdsasigner.key -out ecdsasigner.crt

Get the ECDSA public key from the code signing credentials:
    openssl ec -in ecdsasigner.key -pubout -outform PEM -out ecdsasigner-pub-key.pem


For comprehensive guide on seeting up AWS OTA, please see "AWS OTA User Guide.pdf" document.

Before running the demo it is need to configure AWS IoT Console and update some of project files:

1.  Create AWS Account: https://console.aws.amazon.com/console/home

2.  Configure device in the AWS IoT Console base on this guide: https://docs.aws.amazon.com/iot/latest/developerguide/create-iot-resources.html

    Make note of example's "Thing name" and "REST API endpoint". These strings need to be set in the "aws_clientcredential.h".
    Example:
        #define clientcredentialMQTT_BROKER_ENDPOINT "abcdefgh123456.iot.us-west-2.amazonaws.com"
        #define clientcredentialIOT_THING_NAME "MyExample"

    Device certificate and private key needs to be opened in text editor and its content copied into the "aws_clientcredential_keys.h".
    Note: be sure to add " at the beginning of a line and \n"\ on every line break.
    Example:
        #define keyCLIENT_CERTIFICATE_PEM NULL

        Needs to be changed to:

        #define keyCLIENT_CERTIFICATE_PEM "-----BEGIN CERTIFICATE-----\n"\
        "MIIDWTCCAkGgAwIBAgIUfmv3zA+JULlMOxmz+upkAzhEkQ0wDQYJKoZIhvcNAQEL\n"\
        .
        .
        .
        "mepuT3lKmD0jZupsQ9vLQOA09rMjVMd0YPmI9ozvvWqLpjVvNTKVhsf/3slM\n"\
        "-----END CERTIFICATE-----\n"

    In the same way update "keyCLIENT_PRIVATE_KEY_PEM" with content of private key file and
    "keyCODE_VERIFY_PUB_KEY_PEM" with content of code signing public key file (ecdsasigner-pub-key.pem).

    Files "aws_clientcredential.h" and "aws_clientcredential_keys.h" are located in project folder.    

3.  In case your board connects to the Internet using Wi-Fi, it is necessary to configure also Wi-Fi parameters in "aws_clientcredential.h",
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
