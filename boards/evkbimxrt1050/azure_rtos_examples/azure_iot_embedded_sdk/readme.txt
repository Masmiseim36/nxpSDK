Overview
========
This sample showcases the usability of AZRTOS API to connect to Azure IoT and start interacting with Azure IoT services like IoTHub.
All the output of sample is redirect to stdout. To start the sample, the user needs to provide user configuration in sample_config.h.
The device will periodically report the temperature value to IoTHub.

Prerequisites
1. Register an Azure account. (https://azure.microsoft.com/)
   Note that the name used in the manual can be changed accordingly.

2. Install Azure CLI. Refer to https://docs.microsoft.com/en-us/cli/azure/install-azure-cli?view=azure-cli-latest

3. Create an IoT hub called "imxrthub" in the resource group "imxrthub-test".
   > az login
   > az extension add --name azure-iot
   > az iot hub create --resource-group imxrthub-test --name imxrthub

3. Register a device with the device ID "MyCDevice" in the IoT hub "imxrthub".
   > az extension add --name azure-cli-iot-ext
   > az iot hub device-identity create --hub-name imxrthub --device-id MyCDevice

4. Retrieve your Device Connection String using the Azure CLI.
   Get these parameters from the connectionString:
     HostName (HOST_NAME): imxrthub.azure-devices.net
     DeviceId (DEVICE_ID): MyCDevice
   > az iot hub device-identity show-connection-string --device-id MyCDevice --hub-name imxrthub

5. Make a note of the device connection string, which looks like:
   HostName=imxrthub.azure-devices.net;DeviceId=MyCDevice;SharedAccessKey={YourSharedAccessKey}

6. Write the above device parameters into the source code, rtos/azure-rtos/boards/examples/azure_iot_embedded_sdk/sample_config.h.
   Fill these three macros, HOST_NAME, DEVICE_ID, DEVICE_SYMMETRIC_KEY(={YourSharedAccessKey}).

7. Build the code and write it into the on-board Flash.


Toolchain supported
===================
- IAR embedded Workbench  8.50.9
- Keil MDK  5.33
- GCC ARM Embedded  9.3.1
- MCUXpresso  11.3.0

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

Start the azure_iot_embedded_sdk example...
DHCP In Progress...
IP address: 192.168.0.8
Mask: 255.255.255.0
Gateway: 192.168.0.1
DNS Server address: 192.168.0.1
SNTP Time Sync...
SNTP Time Sync...
SNTP Time Sync...
SNTP Time Sync successfully.
[INFO] Azure IoT Security Module has been enabled, status=0
IoTHub Host Name: imxrthub.azure-devices.net; Device ID: MyCDevice.
Connected to IoTHub.
Telemetry message send: {"Message ID":0}.
Receive twin properties :{"desired":{"GridPowerLimit2":31,"TariffCost":1.28,"$version":62},"reported":{"GridPowerLimit2":33,"TariffCost":2.25,"sample_report":"OK","$version":98}}
Telemetry message send: {"Message ID":1}.
Telemetry message send: {"Message ID":2}.
Telemetry message send: {"Message ID":3}.
Telemetry message send: {"Message ID":4}.
Telemetry message send: {"Message ID":5}.
Telemetry message send: {"Message ID":6}.
Telemetry message send: {"Message ID":7}.
Telemetry message send: {"Message ID":8}.
Telemetry message send: {"Message ID":9}.
Telemetry message send: {"Message ID":10}.

Result
1. If the serial port outputs a message which is similar to the following message, it confirms that the Azure Device Twin function is OK.

   Receive twin properties :{"desired":{"GridPowerLimit2":31,"TariffCost":1.28,"$version":62},"reported":{"GridPowerLimit2":33,"TariffCost":2.25,"sample_report":"OK","$version":98}}

2. If an error message as below appears in the serial output, it's because the board cannot connect to the NTP server. To resolve it, replace SAMPLE_SNTP_SERVER_NAME with an available NTP server in the main.c file.

   SNTP Time Sync failed.

3. If there is no NTP server available, just for testing, the default Epoch time in the SAMPLE_SYSTEM_TIME macro can be updated manually
   in the main.c file. In Linux, use the command to get the current Epoch time, for example:

     $ date +%s
     1610343882

   In Windows 10, use the following command in PowerShell to get the current Epoch time, for example:

     PS C:\Users> (New-TimeSpan -Start (Get-Date 01/01/1970) -End (Get-Date)).TotalSeconds
     1610343882.02736

   Update the value of SAMPLE_SYSTEM_TIME to the current Epoch time, for example, 1610343882. Then, rebuild the project and test it.

