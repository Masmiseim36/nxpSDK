Overview
========
This example demonstrates how to communicate with Azure IoT through MQTT.

Prerequisites
1. Register an Azure account. (https://azure.microsoft.com/)
   Note that the name used in the manual can be changed accordingly.

2. Install Azure CLI. Refer to https://docs.microsoft.com/en-us/cli/azure/install-azure-cli?view=azure-cli-latest

3. Create an IoT hub called "imxrthub" in the resource group "imxrthub-test".
   > az login
   > az extension add --name azure-iot
   > az iot hub create --resource-group imxrthub-test --name imxrthub

3. Register a device with the device ID "imxrt1050" in the IoT hub "imxrthub".
   > az extension add --name azure-cli-iot-ext
   > az iot hub device-identity create --hub-name imxrthub --device-id imxrt1050

4. Retrieve your Device Connection String using the Azure CLI.
   Get these parameters from the connectionString:
     HostName (HOST_NAME): imxrthub.azure-devices.net
     DeviceId (DEVICE_ID): imxrt1050
   > az iot hub device-identity show-connection-string --device-id imxrt1050 --hub-name imxrthub

5. Get a valid IoT hub SAS token. Note that the SAS token will expire in one hour by default.
   sas (DEVICE_SAS): SharedAccessSignature sr=imxrthub.azure-devices.net%2Fdevices%2Fimxrt1050&sig=...
   > az iot hub generate-sas-token -d imxrt1050 -n imxrthub

6. Write the above device parameters into the source code, rtos/azure-rtos/boards/examples/azure_iot_mqtt/sample_azure_iot.c.
   Fill these three macros, HOST_NAME, DEVICE_ID, DEVICE_SAS.

7. Build the code and write it into the on-board Flash.


Toolchain supported
===================
- IAR embedded Workbench  8.50.1
- GCC ARM Embedded  9.2.1
- MCUXpresso  11.2.0

Hardware requirements
=====================
- Mini/micro USB cable
- Network cable RJ45 standard
- EVKB-IMXRT1050 board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Insert Cable to Ethernet RJ45 port and connect it to a ethernet switch.
4.  Write the program to the flash of the target board.
5.  Press the reset button on your board to start the demo.

Running the demo
================
1. When the demo is running, the serial port will output:

Start the azure_iot_mqtt example...
DHCP In Progress...
IP address: 10.193.20.67
Mask: 255.255.255.0
Gateway: 10.193.20.254
DNS Server address: 10.192.130.201
Connected to server
Subscribed to server
[Published] topic = devices/imxrt1050/messages/events/, message: {"temperature": 21}
[Published] topic = devices/imxrt1050/messages/events/, message: {"temperature": 22}
[Published] topic = devices/imxrt1050/messages/events/, message: {"temperature": 23}
[Published] topic = devices/imxrt1050/messages/events/, message: {"temperature": 24}

Customization options
=====================

