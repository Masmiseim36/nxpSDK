Overview
========
This sample showcases the usability of AZRTOS API to connect to Azure IoT and start interacting with Azure IoT services like IoTHub.
All the output of sample is redirect to stdout. To start the sample, the user needs to provide user configuration in sample_config.h.
This example implements the thermostat function with Azure Plug and Play component.
The device will periodically report the temperature value to IoTHub.

Prerequisites
1. Register an Azure account. (https://azure.microsoft.com/)
   Note that the name used in the manual can be changed accordingly.

2. Install Azure CLI. Refer to https://docs.microsoft.com/en-us/cli/azure/install-azure-cli?view=azure-cli-latest

As for following steps, you can refer to https://docs.microsoft.com/en-us/azure/iot-pnp/set-up-environment:
3. Create an IoT hub called "imxrthub" in the resource group "imxrthub-test".
   > az login
   > az extension add --name azure-iot
   > az group create --name imxrthub-test --location centralus
   > az iot hub create --resource-group imxrthub-test --name imxrthub

4. Register a device with the device ID "MyCDevice" in the IoT hub "imxrthub".
   > az extension add --name azure-cli-iot-ext
   > az iot hub device-identity create --hub-name imxrthub --device-id MyCDevice

5. Retrieve your Device Connection String using the Azure CLI.
   Get these parameters from the connectionString:
     HostName (HOST_NAME): imxrthub.azure-devices.net
     DeviceId (DEVICE_ID): MyCDevice
   > az iot hub device-identity show-connection-string --device-id MyCDevice --hub-name imxrthub

6. Make a note of the device connection string, which looks like:
   HostName=imxrthub.azure-devices.net;DeviceId=MyCDevice;SharedAccessKey={YourSharedAccessKey}

7. Write the above device parameters into the source code, rtos/azure-rtos/boards/examples/azure_iot_embedded_sdk/sample_config.h.
   Fill these three macros, HOST_NAME, DEVICE_ID, DEVICE_SYMMETRIC_KEY(={YourSharedAccessKey}).

8. Build the code and write it into the on-board Flash.

9. Install Azure IoT explorer. Refer to https://docs.microsoft.com/en-us/azure/iot-pnp/howto-use-iot-explorer#install-azure-iot-explorer
   The Azure IoT explorer is a graphical tool for interacting with and testing your IoT Plug and Play devices.
   Download link: https://github.com/Azure/azure-iot-explorer/releases

10. Use Azure IoT explorer. Refer to https://docs.microsoft.com/en-us/azure/iot-pnp/howto-use-iot-explorer#use-azure-iot-explorer
   # Download the model file:
     1) Create a folder called 'models' on your local machine
     2) Download https://raw.githubusercontent.com/Azure/opendigitaltwins-dtdl/master/DTDL/v2/samples/Thermostat.json and save the JSON file to the 'models' folder.
   # Connect to Azure IoT Hub:
     1) Retrieve your IoT Hub Connection String using the Azure CLI.
        > az iot hub connection-string show -n my-pnp-hub --key primary --query connectionString
     2) The first time you run the tool, you're prompted for the IoT hub connection string.
   # Configure the tool to use the model files you downloaded previously:
     1) From the home page in the tool, select IoT Plug and Play Settings, then + Add > Local folder.
     2) Select the models folder you created previously.
     3) Then select Save to save the settings.


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
- EVK-IMXRT1020 board
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

Start the azure_iot_embedded_sdk_pnp example...
DHCP In Progress...
IP address: 192.168.31.99
Mask: 255.255.255.0
Gateway: 192.168.31.1
DNS Server address: 192.168.31.1
SNTP Time Sync...
SNTP Time Sync...
SNTP Time Sync...
SNTP Time Sync successfully.
[INFO] Azure IoT Security Module has been enabled, status=0
Connected to IoTHub.
Telemetry message send: {"temperature":22}.
Receive twin properties: {"desired":{"status":"OK","targetTemperature":30,"$version":4},"reported":{"sample_report":"OK","maxTempSinceLastReboot":30,"targetTemperature":{"value":30,"ac":200,"av":4,"ad":"success"},"serialNumber":"serial-no-123-abc","deviceInformation":{"__t":"c","manufacturer":"Sample-Manufacturer","model":"pnp-sample-Model-123","swVersion":"1.0.0.0","osName":"AzureRTOS","processorArchitecture":"Contoso-Arch-64bit","processorManufacturer":"Processor Manufacturer(TM)","totalStorage":1024,"totalMemory":128},"thermostat1":{"__t":"c","maxTempSinceLastReboot":22},"thermostat2":{"__t":"c","maxTempSinceLastReboot":22},"$version":22}}
Telemetry message send: {"temperature":30}.
Telemetry message send: {"temperature":30}.
Telemetry message send: {"temperature":30}.
Telemetry message send: {"temperature":30}.
Telemetry message send: {"temperature":30}.
Telemetry message send: {"temperature":30}.
Telemetry message send: {"temperature":30}.
Telemetry message send: {"temperature":30}.
Telemetry message send: {"temperature":30}.

Result
1. If the serial port outputs a message which is similar to the following message, it confirms that the Azure Device Twin function is OK.

   Receive twin properties: {"desired":{"status":"OK","targetTemperature":30,"$version":4},"reported":{"sample_report":"OK","maxTempSinceLastReboot":30,"targetTemperature":{"value":30,"ac":200,"av":4,"ad":"success"},"serialNumber":"serial-no-123-abc","deviceInformation":{"__t":"c","manufacturer":"Sample-Manufacturer","model":"pnp-sample-Model-123","swVersion":"1.0.0.0","osName":"AzureRTOS","processorArchitecture":"Contoso-Arch-64bit","processorManufacturer":"Processor Manufacturer(TM)","totalStorage":1024,"totalMemory":128},"thermostat1":{"__t":"c","maxTempSinceLastReboot":22},"thermostat2":{"__t":"c","maxTempSinceLastReboot":22},"$version":22}}

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

