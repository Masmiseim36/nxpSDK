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
- Version: 2.15.000

Toolchain supported
===================
- IAR embedded Workbench  9.40.1
- Keil MDK  5.38.1
- GCC ARM Embedded  12.2
- MCUXpresso  11.8.0

Hardware requirements
=====================
- Micro USB cable
- evkbmimxrt1060 board
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
To get the application properly executed by the bootloader, it is necessary to put signed application image to the primary application partition.
There are multiple options how to achieve that, however in principle the are two main methods (both presuming the bootlaoder is already in place):

a) programing signed application image to the primary application partition using an external tool (direct method)
b) jump-starting the application by debugger, performing OTA update with the signed image, resetting the board and letting the bootloader to perform update (indirect method)

The latter method is described step by step below:

1.  Load the demo project and build it.
    Known issue: MDK linker issues warning about unused boot_hdr sections. This does not affect the functionality of the example.
    
2.  Prepare signed image of the application from raw binary as described in the mcuboot_opensource readme.
     - In case of MCUXpress raw binary may not be generated automatically. Use binary tools after right clicking Binaries/.axf file in the project tree to generate it manually.
    
3.  Launch the debugger in your IDE to jump-start the application.
     - In case of MCUXpresso IDE the execution stalls in an endless loop in the bootloader. Pause the debugging and use debugger console and issue command 'jump ResetISR'.

4.  The OTA demo connects to the cloud service and expects an update package to be deployed.
    Plese refer to https://docs.aws.amazon.com/freertos/latest/userguide/ota-manager.html on how to create an update job with the signed image.
    Note: the image signed for MCUBoot (using imgtool) needs to be further signed for AWS OTA job. These are distinct signatures that are being checked at different stages of the OTA/boot process.
    
5.  After the OTA agent (that is part the the demo) gets notification about the update it starts downloading the image and storing it into secondary application partition.

6.  Once the image is downloaded, bootloader is notified about it and reboot in test-mode takes place.
    If the updated firmware is able to run without issues the update is made permanent automatically.
