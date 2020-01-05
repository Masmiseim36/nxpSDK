Overview
========
Azure_mqtt demo shows basic communication with iot-hub via mqtt protocol.


Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- MCUXpresso  11.1.0
- GCC ARM Embedded  8.3.1
- Keil MDK  5.29

Hardware requirements
=====================
- Mini/micro USB cable
- RJ45 Network cable
- FRDM-K64F board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
---------------------------------------------------------
Known issue in MCUXpresso IDE v11.0.0_alpha [Build 2495]:
MCUXpresso IDE import wrong value of SDK_DEBUGCONSOLE macro. Manual change of configuration macro SDK_DEBUGCONSOLE is required after project import.
Perform following steps:
1. In menu Quick Settings>> -> Defined symbols edit value of macro "SDK_DEBUGCONSOLE=1" to "SDK_DEBUGCONSOLE=0".
2. After the step 1, new definition "SDK_DEBUGCONSOLE=1" automatically appear in the menu. Delete it, keep only "SDK_DEBUGCONSOLE=0".
3. In menu Quick Settings>> -> Defined symbols define new macro "SDK_DEBUGCONSOLE_UART".
---------------------------------------------------------

Prior to running the samples, you will need to have an instance of Azure IoT Hub available
and a device Identity created in the hub.

Setup your IoT hub
[https://github.com/Azure/azure-iot-device-ecosystem/blob/master/setup_iothub.md]

Edit the iothub_client_sample_mqtt.c file entering the proper connection string from the Azure Portal:
static const char* connectionString = "[device connection string]";

1.  Connect a USB cable between the PC host and the OpenSDA(or USB to Serial) USB port on the target board.
2.  Open a serial terminal on PC for OpenSDA serial(or USB to Serial) device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Insert the Ethernet Cable into the target board's RJ45 port and connect it to your PC network adapter.
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
After the board is flashed the Tera Term will start printing the state of event bits.

Example output:
Initializing PHY...
 DHCP state       : SELECTING
 DHCP state       : REQUESTING
 DHCP state       : CHECKING
 DHCP state       : BOUND

 IPv4 Address     : xxx.xxx.xxx.xxx
 IPv4 Subnet mask : xxx.xxx.xxx.xxx
 IPv4 Gateway     : xxx.xxx.xxx.xxx

Info: Initializing SNTP
Info: SNTP initialization complete
IoTHubClient_LL_SetMessageCallback...successful.
IoTHubClient_LL_SendEventAsync accepted message [0] for transmission to IoT Hub.
-> 15:56:54 CONNECT | VER: 4 | KEEPALIVE: 240 | FLAGS: 192 | USERNAME: MCU-IOT-Hub.azure-devices.net/frdmk64f/api-version=2016-11-14&DeviceClientType=iothubclient%2f1.2.3%20(native%3b%20FreeRTOS%3b%20MSDK) | PWD: XXXX | CLEAN: 0
IoTHubClient_LL_SendEventAsync accepted message [1] for transmission to IoT Hub.
IoTHubClient_LL_SendEventAsync accepted message [2] for transmission to IoT Hub.
IoTHubClient_LL_SendEventAsync accepted message [3] for transmission to IoT Hub.
IoTHubClient_LL_SendEventAsync accepted message [4] for transmission to IoT Hub.
<- 15:56:54 CONNACK | SESSION_PRESENT: true | RETURN_CODE: 0x0
-> 15:56:54 SUBSCRIBE | PACKET_ID: 2 | TOPIC_NAME: devices/frdmk64f/messages/devicebound/# | QOS: 1
-> 15:56:54 PUBLISH | IS_DUP: false | RETAIN: 0 | QOS: DELIVER_AT_LEAST_ONCE | TOPIC_NAME: devices/frdmk64f/messages/events/temperatureAlert=true&%24.cid=CORE_ID&%24.mid=MSG_ID&%24.ct=application%2Fjson&%24.ce=utf-8 | PACKET_ID: 3 | PAYLOAD_LEN: 74
-> 15:56:54 PUBLISH | IS_DUP: false | RETAIN: 0 | QOS: DELIVER_AT_LEAST_ONCE | TOPIC_NAME: devices/frdmk64f/messages/events/temperatureAlert=true&%24.cid=CORE_ID&%24.mid=MSG_ID&%24.ct=application%2Fjson&%24.ce=utf-8 | PACKET_ID: 4 | PAYLOAD_LEN: 74
-> 15:56:55 PUBLISH | IS_DUP: false | RETAIN: 0 | QOS: DELIVER_AT_LEAST_ONCE | TOPIC_NAME: devices/frdmk64f/messages/events/temperatureAlert=false&%24.cid=CORE_ID&%24.mid=MSG_ID&%24.ct=application%2Fjson&%24.ce=utf-8 | PACKET_ID: 5 | PAYLOAD_LEN: 74
-> 15:56:55 PUBLISH | IS_DUP: false | RETAIN: 0 | QOS: DELIVER_AT_LEAST_ONCE | TOPIC_NAME: devices/frdmk64f/messages/events/temperatureAlert=false&%24.cid=CORE_ID&%24.mid=MSG_ID&%24.ct=application%2Fjson&%24.ce=utf-8 | PACKET_ID: 6 | PAYLOAD_LEN: 74
-> 15:56:55 PUBLISH | IS_DUP: false | RETAIN: 0 | QOS: DELIVER_AT_LEAST_ONCE | TOPIC_NAME: devices/frdmk64f/messages/events/temperatureAlert=false&%24.cid=CORE_ID&%24.mid=MSG_ID&%24.ct=application%2Fjson&%24.ce=utf-8 | PACKET_ID: 7 | PAYLOAD_LEN: 74
<- 15:56:55 SUBACK | PACKET_ID: 2 | RETURN_CODE: 1
<- 15:56:56 PUBACK | PACKET_ID: 3
Confirmation[0] received for message tracking id = 0 with result = IOTHUB_CLIENT_CONFIRMATION_OK
<- 15:56:56 PUBACK | PACKET_ID: 4
Confirmation[1] received for message tracking id = 1 with result = IOTHUB_CLIENT_CONFIRMATION_OK
<- 15:56:56 PUBACK | PACKET_ID: 5
Confirmation[2] received for message tracking id = 2 with result = IOTHUB_CLIENT_CONFIRMATION_OK
<- 15:56:56 PUBACK | PACKET_ID: 6
Confirmation[3] received for message tracking id = 3 with result = IOTHUB_CLIENT_CONFIRMATION_OK
<- 15:56:56 PUBACK | PACKET_ID: 7
Confirmation[4] received for message tracking id = 4 with result = IOTHUB_CLIENT_CONFIRMATION_OK




Customization options
=====================

