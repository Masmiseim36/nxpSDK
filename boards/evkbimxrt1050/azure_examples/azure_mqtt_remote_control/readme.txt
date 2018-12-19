Overview
========
Azure_mqtt_remote_control demo shows basic communication with iot-hub via mqtt protocol.

Running the demo
================
After the board is flashed the Tera Term will start printing the state of event bits.

Example output:
 DHCP state       : SELECTING
 DHCP state       : REQUESTING
 DHCP state       : CHECKING
 DHCP state       : BOUND

 IPv4 Address     : xxx.xxx.xxx.xxx
 IPv4 Subnet mask : xxx.xxx.xxx.xxx
 IPv4 Gateway     : xxx.xxx.xxx.xxx

Info: Initializing SNTP
Info: SNTP initialization complete
Starting the IoTHub client sample HTTP...
IoTHubClient_LL_SetMessageCallback...successful.
Hardware requirements
=====================
- Mini/micro USB cable
- RJ45 Network cable
- EVKB-IMXRT1050 board
- Personal Computer

Board settings
==============
Connect a mini USB cable between the PC host and the OpenSDA USB port.
No special settings are required.

Prepare the Demo
================
Prior to running the samples, you will need to have an instance of Azure IoT Hub available
and a device Identity created in the hub.

Setup your IoT hub
[https://github.com/Azure/azure-iot-device-ecosystem/blob/master/setup_iothub.md]

Edit the remote_control_mqtt.c file entering the proper connection string from the Azure Portal:
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


Toolchain supported
===================
- IAR embedded Workbench  8.32.1
- Keil MDK  5.26
- GCC ARM Embedded  7.3.1
- MCUXpresso 10.3.0

