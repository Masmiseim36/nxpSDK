Hardware requirements
=====================
- Mini/micro USB cable
- EVKB-MIMXRT1170 board with adapter
- Personal Computer
- Network cable RJ45 standard (Network with Internet access)
- OM- SE050ARD or OM- SE051ARD

Board settings
==============
Jumper settings for OM- SE050ARD & OM- SE051ARD:
    -J14 3-4 (Default): EdgeLock SE05x_VCC pin routed to SE05X_VOUT pin
    -J15 1-2: I2C target SDA connection
    -J17 1-2: I2C target SCL connection

Prerequisites
==============
- Active AWS account
- The Plug & Trust MW package can be downloaded from
https://www.nxp.com/products/security-and-authentication/authentication/edgelock-se050-plug-and-trust-secure-element-family-enhanced-iot-security-with-high-flexibility:SE050#design-resources
- Any Serial communicator
- Flash VCOM binary on the device at flash address 0x30000000. VCOM binary se05x_vcom-T1oI2C-<board>.bin can found in Plug & Trust MW package :simw-top\binaries\MCU folder.

Creating and updating device keys and certificates to SE
=========================================================

1. Check the vcom port number
2. Go to the EdgeLock SE05x Plug & Trust middleware ..\simw-top\binaries\PCWindows\ssscli folder and call:

    Provision_AWS.exe <COMxx>
    note:
    <COMxx> is detected vcom port
3. Certificates and Keys are generated at `simw-top/binaries/PCWindows/ssscli/aws`

Creating a device on AWS account
=========================================

Refer - https://docs.aws.amazon.com/iot/latest/developerguide/iot-gs-first-thing.html

Running the Demo
================

1. To get the AWS IoT MQTT broker endpoint for your account, go to the AWS IoT console and in the left navigation pane choose Settings. Copy the endpoint listed under the "Device data endpoint"

2. In the '<PROJECT>\source\aws_iot_config.h' file, update the endpoint in the macro "clientcredentialMQTT_BROKER_ENDPOINT"

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

[INFO] Create a TCP connection to a29rg0ytflhg6y.iot.eu-central-1.amazonaws.com:                                                                                                                                                             8883.
[INFO] (Network connection 0x20208970) TLS handshake successful.
[INFO] (Network connection 0x20208970) Connection to a29rg0ytflhg6y.iot.eu-centr                                                                                                                                                             al-1.amazonaws.com [INFO] MQTT connection established with the broker.
[INFO] MQTT connection successfully established with broker.


[INFO] A clean MQTT connection is established. Cleaning up all the stored outgoing publishes.


App   :INFO :MQTT Connection successfully established
[INFO] Attempt to subscribe to the MQTT topic $aws/things/K64f_1/shadow/update/accepted.
App   :INFO :MQTT Subscribed
[INFO] the published payload:{"msg" : "hello from SDK QOS0 : 0"}

[INFO] PUBLISH sent for topic sdkTest/sub to broker with packet ID 2.


App   :INFO :Published message {"msg" : "hello from SDK QOS0 : 0"}
App   :INFO :AWS demo passed
App   :INFO :Demo Example Finished


