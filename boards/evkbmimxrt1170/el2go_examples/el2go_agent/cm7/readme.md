# EdgeLock 2GO Agent

This sample application shows how to use the EdgeLock 2GO service for provisioning keys and certificates into the SE05x secure element. Those keys and certificates can then be used to establish mutual-authenticated TLS connections to cloud services such as AWS or Azure.

## Prerequisites

- Active [EdgeLock 2GO](https://www.nxp.com/products/security-and-authentication/secure-service-2go-platform/edgelock-2go:EDGELOCK-2GO) account
- Any serial communicator

## Setup of the EdgeLock 2GO service

The documentation which explains how to setup the EdgeLock 2GO Account to

- Create a device group and whitelist the device UUID
- Create secure objects
- Assign the secure objects to the device group

can be found on the Edgelock 2GO web interface under the "Documentation" tab.

## Hardware requirements

- MIMXRT1060-EVKB, MIMXRT1060-EVKC or MIMXRT1170-EVKB board
- OM-SE050ARD or OM-SE051ARD Development Kit
- Micro-USB cable
- Ethernet cable
- Personal Computer

## Board settings

Connect the OM-SE050ARD or OM-SE051ARD Development Kit to your board.

#### MIMXRT1060-EVKB & MIMXRT1060-EVKC

Jumper settings for OM-SE050ARD or OM-SE051ARD:
- J14 3-4 (Default): EdgeLock SE05x_VCC pin routed to SE05X_VOUT pin
- J15 3-4 (Default): I2C target SDA connection
- J17 3-4 (Default): I2C target SCL connection

#### MIMXRT1170-EVKB

Jumper settings for OM-SE050ARD or OM-SE051ARD:
- J14 3-4 (Default): EdgeLock SE05x_VCC pin routed to SE05X_VOUT pin
- J15 1-2: I2C target SDA connection
- J17 1-2: I2C target SCL connection

## Preparing the application

1. Refer to the SE05x user manual and Plug & Trust Middleware documentation on how to prepare your device for cloud connectivity demos. They can be found in the [EdgeLock SE05x Plug & Trust Middleware package](https://www.nxp.com/products/security-and-authentication/authentication/:SE050) ("Software->NXP" section).

2.  Provide the EdgeLock 2GO URL for the account (can be found in the "Admin" section):

    [middleware/nxp_iot_agent/inc/nxp_iot_agent_config.h](../../../middleware/nxp_iot_agent/inc/nxp_iot_agent_config.h)

    ```c
    #define EDGELOCK2GO_HOSTNAME
    ```

3.  **[OPTIONAL]** Enable MQTT demo:

    In case you want to use provisioned ECC key pairs and corresponding X.509 certificates to execute TLS mutual-authentication and MQTT message exchange with AWS and/or Azure clouds, enable the corresponding macro.

    [middleware/nxp_iot_agent/ex/inc/iot_agent_demo_config.h](../../../middleware/nxp_iot_agent/ex/inc/iot_agent_demo_config.h)

    ```c
    #define IOT_AGENT_MQTT_ENABLE 1
    ```

    In the same file, the AWS and/or Azure macros should be set to the object IDs as defined at the EdgeLock 2GO service.

    ```c
    #define AWS_SERVICE_KEY_PAIR_ID
    #define AWS_SERVICE_DEVICE_CERT_ID
    ...
    #define AZURE_SERVICE_KEY_PAIR_ID
    #define AZURE_SERVICE_DEVICE_CERT_ID
    ```

    The settings of other macros are server dependent and their meaning can be found in the AWS/Azure documentation. By default, the demo is executing a connection to both clouds when `IOT_AGENT_MQTT_ENABLE` is enabled; To enable or disable them individually, use the `AWS_ENABLE` and `AZURE_ENABLE` macros respectively.

4.  Build the application.

5.  Connect the Micro-USB cable to the PC host and the MCU-Link USB port (J41 [MIMXRT1060-EVKB or MIMXRT1060-EVKC] or J86 [MIMXRT1170-EVKB]) on the board.

6.  Open a serial terminal with the following settings:

    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control

7.  Connect the Ethernet cable to the RJ45 port (Use "1G ENET" on MIMXRT1170-EVKB).

8.  Download the application to the target board.

9.  Press the reset button on the board or launch the debugger in your IDE to run the application.

## Running the application

The log below shows the output of the application in the terminal window (with MQTT connection to Azure service).

Initializing a session with SE05x and printing the UUID:

```
Initializing PHY...
Performance timing: DEVICE_INIT_TIME : 4002ms
Start
sss   :INFO :atr (Len=35)
                01 A0 00 00     03 96 04 03     E8 00 FE 02     0B 03 E8 00
                01 00 00 00     00 64 13 88     0A 00 65 53     45 30 35 31
                00 00 00
sss   :WARN :Communication channel is Plain.
sss   :WARN :!!!Not recommended for production use.!!!
UID in hex format: 040050017B65A28FFA27F4047559221F7080
UID in decimal format: 348555489661789374683563581185171472216192
```

Connecting to the EdgeLock 2GO service and printing the report:

```
Updating device configuration from [xxx.device-link.edgelock2go.com]:[443].
Update status report:
  The device update was successful (0x0001: SUCCESS)
  The correlation-id for this update is 2b971c9c-b046-4b7b-8062-ca5bc13f7b20.
  Status for remote trust provisioning: 0x0001: SUCCESS.
    On endpoint 0x70000010, for object 0x83000212, status: 0x0001: SUCCESS
    On endpoint 0x70000010, for object 0x83000211, status: 0x0001: SUCCESS.
Found configuration data for 0 services.
Performance timing: ENTIRE_SESSION_TIME : 5306ms
        Performance timing: AGENT_INIT_TIME : 28ms
        Performance timing: TLS_PREP_TIME : 14ms
        Performance timing: NETWORK_CONNECT_TIME : 1230ms
        Performance timing: PROCESS_PROVISION_TIME : 3990ms
        CRL_TIME : [65ms] and COMMAND_TXRX_TIME : [0ms] included in PROCESS_PROVISION_TIME
```

Connecting to an Azure service using the provisioned ECC key pair and corresponding X.509 certificate:

```
MQTT attempting to register Azure Service 'azuretest0000000000edca11-0000' (the operation migth take around 1 minute)...
[INFO] (Network connection 0x200047f0) TLS handshake successful.
[INFO] (Network connection 0x200047f0) Connection to global.azure-devices-provisioning.net establis[INFO] MQTT connection established with the broker.
Subscription ack message received
[INFO] De-serialized incoming PUBLISH packet: DeserializerResult=MQTTSuccess.
[INFO] State record updated. New state=MQTTPublishDone.
Publish message received on topic
Device State is now ASSIGNING
[INFO] De-serialized incoming PUBLISH packet: DeserializerResult=MQTTSuccess.
[INFO] State record updated. New state=MQTTPublishDone.
Publish message received on topic
Device State is now ASSIGNED
State is ASSIGNED
[INFO] Disconnected from the broker.
[INFO] (Network connection 0x200047f0) TLS close-notify sent.
Attempt 0 for connecting to Azure service 'azuretest0000000000edca11-0000'...
[INFO] (Network connection 0x200047f0) TLS handshake successful.
[INFO] (Network connection 0x200047f0) Connection to IotHub-DL-EL2GO-E2ETests.azure-devices.net est[INFO] MQTT connection established with the broker.
Echo successfully published
Echo successfully published
Echo successfully published
Echo successfully published
```

## Notes

If the EdgeLock 2GO server finds an already existing object with the same ID on the device during provisioning, it will first delete the object before provisioning the new one.

The following limitations apply when deleting an object during provisioning:
- The server doesn't know the DELETE permissions of the object on the device (which are defined through policies).
- The server can't read the policies using a "read attribute" APDU, since the READ permissions might be disabled.

To solve the issue, the server executes two DELETE operations, one unathenticated and one authenticated, which covers most of the possible object status on the device. 
Thus, if you have an object where the unauthenticated DELETE is not allowed while the authenticated is, the first APDU will fail with the 6986 status word. The server will ignore the failure and continue with provisioning, but on the device, a warning will be displayed:

```
sss   :WARN :APDU Transaction Error: Command not allowed - access denied based on object policy (0x6986)
```

You can ignore this and similar warnings in case the final report shows "SUCCESS".
