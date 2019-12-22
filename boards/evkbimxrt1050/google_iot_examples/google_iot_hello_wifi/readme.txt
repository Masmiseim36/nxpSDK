Overview
========
This example demonstrates how the board can connect to Google Cloud IoT Core
device using MQTT bridge and send messages to this device.


Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- GCC ARM Embedded  8.3.1
- MCUXpresso  11.1.0

Hardware requirements
=====================
- Micro USB cable
- evkbimxrt1050 board
- Personal Computer
- Murata 1DX M.2 module
- Murata uSD M.2 Adapter
- 1 plug to receptable header cable

Board settings
==============
The following pins between the evkbimxrt1050 board and Murata uSD M.2 Adapter are connected using the plug to receptable cables:
- evkbimxrt1050 board's connector J22, pin 3 to Murata uSD M.2 Adapter's connector J9, pin 3
Prepare the Demo
================
Before running the demo it is necessary to configure Google Cloud Platform Console and update some of the project files:

1.  Create GCP Account: https://cloud.google.com

2.  Generate an ES256 key with a self-signed X.509 certificate following this guide:
    https://cloud.google.com/iot/docs/how-tos/credentials/keys
    Section "Generating an ES256 key with a self-signed X.509 certificate"
    An openssl command-line tool is necessary for generation.

3.  Create Project, Registry and Device in the cloud platform following this guide:
    https://cloud.google.com/iot/docs/how-tos/devices
    Make note of of the project ID, registry ID, region and device ID.

4.  Insert the pregenerated public key to the created GCP device

5.  Open example's project, modify google_iot_config.h file in config folder with appropriate credentials:
    - paste the private key that was generated in step 2
    - insert project ID, registry ID, region and device ID to appropriate defines
    - update the WiFi SSID and password
    - build the project

6.  Connect a USB cable between the PC host and the OpenSDA USB port on the target board.

7.  Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control

8.  Connect the WiFi module to the micro SD card slot. IP address is assigned by the DHCP server.
    Make sure the connection on port 8883 for MQTT and 123 for NTP is not blocked.

9.  Download the program to the target board.

10.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.


Running the demo
================
The log below shows the output of the demo in the terminal window.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Google Cloud Platform IoT Hello example start.
Initializing WiFi connection...

 AsyncInterrupt is not supported
WLAN MAC Address : A0:C9:A0:3D:F9:2F
WLAN Firmware    : wl0: Feb 12 2018 04:08:14 version 7.79.2 (r683798 CY) FWID 01-27b63357
WLAN CLM         : API: 12.2 Data: 9.10.39 Compiler: 1.29.4 ClmImport: 1.36.3 Creation: 2018-02-12 04:00:50
Successfully initialized WiFi module
Joining: nxp
Successfully joined: nxp
Getting IP address from DHCP server
 IPv4 Address got from DHCP  : 192.168.2.103
Starting Custom Application Logic Task...
[ custom application logic ] [ heap left: 38192 bytes ]
Starting GCP IoT Embedded C Client...
Initializing SNTP
[ custom application logic ] [ heap left: 38192 bytes ]
[ custom application logic ] [ heap left: 38192 bytes ]
[ custom application logic ] [ heap left: 38192 bytes ]
SNTP initialization complete
Actual UTC time: Wed Jun  5 11:13:40 2019

.[ custom application logic ] [ heap left: 5656 bytes ]
connected to mqtt.2030.ltsapis.goog:8883
publishing msg "Hello From Your IoTC client!" to topic: "/devices/test-device/state"
[ custom application logic ] [ heap left: 8296 bytes ]
[ custom application logic ] [ heap left: 8296 bytes ]
[ custom application logic ] [ heap left: 8296 bytes ]
[ custom application logic ] [ heap left: 8280 bytes ]
[ custom application logic ] [ heap left: 8280 bytes ]
publishing msg "Hello From Your IoTC client!" to topic: "/devices/test-device/state"
[ custom application logic ] [ heap left: 8280 bytes ]
[ custom application logic ] [ heap left: 8280 bytes ]
[ custom application logic ] [ heap left: 8280 bytes ]
[ custom application logic ] [ heap left: 8176 bytes ]
[ custom application logic ] [ heap left: 8264 bytes ]
publishing msg "Hello From Your IoTC client!" to topic: "/devices/test-device/state"
[ custom application logic ] [ heap left: 8264 bytes ]
[ custom application logic ] [ heap left: 8264 bytes ]
[ custom application logic ] [ heap left: 8264 bytes ]
[ custom application logic ] [ heap left: 8184 bytes ]
[ custom application logic ] [ heap left: 8280 bytes ]
[ custom application logic ] [ heap left: 8280 bytes ]
publishing msg "Hello From Your IoTC client!" to topic: "/devices/test-device/state"
[ custom application logic ] [ heap left: 8280 bytes ]
[ custom application logic ] [ heap left: 8280 bytes ]
[ custom application logic ] [ heap left: 8280 bytes ]
[ custom application logic ] [ heap left: 8264 bytes ]
[ custom application logic ] [ heap left: 8264 bytes ]
publishing msg "Hello From Your IoTC client!" to topic: "/devices/test-device/state"
[ custom application logic ] [ heap left: 8264 bytes ]
[ custom application logic ] [ heap left: 8264 bytes ]
[ custom application logic ] [ heap left: 8264 bytes ]
[ custom application logic ] [ heap left: 8280 bytes ]
publishing msg "Hello From Your IoTC client!" to topic: "/devices/
.
.
.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

