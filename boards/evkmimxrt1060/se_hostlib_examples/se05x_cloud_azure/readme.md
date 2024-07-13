
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
- Mini/micro USB cable
- EVK-MIMXRT1060 board
- Personal Computer
- Network cable RJ45 standard (Network with Internet access)
- OM- SE050ARD or OM- SE051ARD

Board settings
==============
Jumper settings for OM- SE050ARD & OM- SE051ARD:
    -J14 3-4 (Default): EdgeLock SE05x_VCC pin routed to SE05X_VOUT pin
    -J15 3-4 (Default): I2C target SDA connection
    -J17 3-4 (Default): I2C target SCL connection

Prerequisites
- Active azure account with iot hub created
- The Plug & Trust MW package can be downloaded from
https://www.nxp.com/products/security-and-authentication/authentication/edgelock-se050-plug-and-trust-secure-element-family-enhanced-iot-security-with-high-flexibility:SE050#design-resources
- Any Serial communicator
- Flash VCOM binary on the device at flash address 0x60000000. VCOM binary se05x_vcom-T1oI2C-<board>.bin can found in Plug & Trust MW package :simw-top\binaries\MCU folder.

Creating a device on azure IoT Hub portal

1.  Navigate to the Dashboard --> <Your IoT Hub> --> "Devices"
2.  Add a new device (e.g. device1), and for its authentication type
    choose X.509 CA Signed

Creating and updating device keys and certificates to SE

1. Check the vcom port number
2. Go to the EdgeLock SE05x Plug & Trust middleware ..\simw-top\binaries\PCWindows\ssscli folder and call:
   
    Provision_AZURE.exe <COMxx> 
    note: 
    <COMxx> is detected vcom port
3. Certificates and Keys are generated at `simw-top/binaries/PCWindows/ssscli/azure`

Uploading root certificates to IoT Hub

1. On Azure IoT Hub portal, Navigate to ``Dashboard --> <Your IoT Hub> --> Certificates``. Click on Add
2. Enter a friendly name and upload the root certificate created in the previous step. Location -  ``simw-top\pycli\Provisioning\azure\RootCA.cer`` -> Save
3. Your certificate will show in the Certificate Explorer list. Click on certificate added
4. In Certificate Details, click Generate Verification Code
5. The provisioning service creates a Verification Code that you can use to validate the certificate ownership. Copy the code to your clipboard
6. Use the verification_certificate.py to generate a verify certificate (verifyCert4.cer) ::

        cd simw-top\pycli\Provisioning
        python verification_certificate.py <RootCA_Certificate> <RootCA_Keypair> <Verification Code>

7. On ``Azure portal -> Certificate Details``, upload the verifyCert4.cer file generated and click Verify.
   STATUS of your certificate should change to ``Verified`` in the Certificate Explorer list

Running the Demo

1. Update ``AZURE_IOT_HUB_NAME``  and ``AZURE_DEVICE_NAME`` in ``<PROJECT>\source\azure_iot_config.h`` with your details.
2. Update ``clientcredentialWIFI_SSID`` and ``clientcredentialWIFI_PASSWORD`` in ``<PROJECT>\se_hostlib\demos\ksdk\common\wifi_config.h`` with your wifi credentials.
3. Connect your board to open network on ethernet.
4. Build the project and flash the binary on board.

The log below shows the output of the demo in the terminal window. The log can be different based on your local network configuration.
App   :INFO :PlugAndTrust_v04.04.00_20230811
sss   :INFO :atr (Len=35)
                01 A0 00 00     03 96 04 03     E8 00 FE 02     0B 03 E8 00
                01 00 00 00     00 64 13 88     0A 00 65 53     45 30 35 31
                00 00 00
sss   :WARN :Communication channel is Plain.
sss   :WARN :!!!Not recommended for production use.!!!
App   :INFO :AWS subscribe publish example
 
 
Initializing PHY...
App   :INFO :Getting IP address from DHCP ...
 
App   :INFO :
IPv4 Address     : 192.168.199.7
 
App   :INFO :DHCP OK

[INFO] Create a TCP connection to NXPPNT.azure-devices.net:8883.
[INFO] (Network connection 0x20009288) TLS handshake successful.
[INFO] (Network connection 0x20009288) Connection to NXPPNT.azure-devices.net established.
[INFO] MQTT connection established with the broker.
[INFO] MQTT connection successfully established with broker.

[INFO] A clean MQTT connection is established. Cleaning up all the stored outgoing publishes.

App   :INFO :MQTT Connection successfully established
[INFO] Attempt to subscribe to the MQTT topic devices/377819357023377568295309/messages/devicebound[DEBUG] Encoded size for length 60 is 1 bytes.

App   :INFO :MQTT Subscribed
[INFO] the published payload:{"msg" : "hello from SDK QOS0 : 0"}
[INFO] PUBLISH sent for topic devices/377819357023377568295309/messages/events/ to broker with packApp   :INFO :Published message {"msg" : "hello from SDK QOS0 : 0"}
App   :INFO :AZURE demo passed
App   :INFO :Demo Example Finished
