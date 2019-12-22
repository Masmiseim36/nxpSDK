Overview
========
This example demonstrates how the board can connect to Google Cloud IoT Core
device using MQTT bridge and send messages to this device.


Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- Keil MDK  5.29
- GCC ARM Embedded  8.3.1
- MCUXpresso  11.1.0

Hardware requirements
=====================
- Micro USB cable
- EVK-MIMXRT1060 board
- Personal Computer
- Network cable RJ45 standard (Network with Internet access)

Board settings
==============
No special settings are required.

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
    - build the project

6.  Connect a USB cable between the PC host and the OpenSDA USB port on the target board.

7.  Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control

8.  Connect the board's RJ45 to network with Internet access. IP address is assigned by the DHCP server.
    Make sure the connection on port 8883 for MQTT and 123 for NTP is not blocked.

9.  Download the program to the target board.

10.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.


Running the demo
================
The log below shows the output of the demo in the terminal window.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Google Cloud Platform IoT Hello example start.
Initializing PHY...
Getting IP address from DHCP ...
IPv4 Address: 10.42.0.61
DHCP OK
Starting Custom Application Logic Task...
[ custom application logic ] [ heap left: 41968 bytes ]
Starting GCP IoT Embedded C Client...
Initializing SNTP
[ custom application logic ] [ heap left: 41968 bytes ]
[ custom application logic ] [ heap left: 41968 bytes ]
[ custom application logic ] [ heap left: 41968 bytes ]
SNTP initialization complete
Actual UTC time: Wed Jun  5 11:22:50 2019

.[ custom application logic ] [ heap left: 9112 bytes ]
[ custom application logic ] [ heap left: 8880 bytes ]
connected to mqtt.2030.ltsapis.goog:8883
publishing msg "Hello From Your IoTC client!" to topic: "/devices/test-device/state"
[ custom application logic ] [ heap left: 12088 bytes ]
[ custom application logic ] [ heap left: 12088 bytes ]
[ custom application logic ] [ heap left: 12088 bytes ]
[ custom application logic ] [ heap left: 11984 bytes ]
[ custom application logic ] [ heap left: 11984 bytes ]
[ custom application logic ] [ heap left: 12072 bytes ]
publishing msg "Hello From Your IoTC client!" to topic: "/devices/test-device/state"
[ custom application logic ] [ heap left: 12072 bytes ]
[ custom application logic ] [ heap left: 12072 bytes ]
[ custom application logic ] [ heap left: 12072 bytes ]
[ custom application logic ] [ heap left: 12056 bytes ]
[ custom application logic ] [ heap left: 12056 bytes ]
publishing msg "Hello From Your IoTC client!" to topic: "/devices/test-device/state"
[ custom application logic ] [ heap left: 12056 bytes ]
[ custom application logic ] [ heap left: 12056 bytes ]
[ custom application logic ] [ heap left: 12056 bytes ]
[ custom application logic ] [ heap left: 12072 bytes ]
[ custom application logic ] [ heap left: 12072 bytes ]
publishing msg "Hello From Your IoTC client!" to topic: "/devices/test-device/state"
[ custom application logic ] [ heap left: 12072 bytes ]
[ custom application logic ] [ heap left: 12072 bytes ]
[ custom application logic ] [ heap left: 12072 bytes ]
[ custom application logic ] [ heap left: 12056 bytes ]
publishing msg "Hello From Your IoTC client!" to topic: "/devices/test-device/state"
[ custom application logic ] [ heap left: 12056 bytes ]
[ custom application logic ] [ heap left: 12056 bytes ]
[ custom application logic ] [ heap left: 12056 bytes ]
[ custom application logic ] [ heap left: 12072 bytes ]
[ custom application logic ] [ heap left: 12072 bytes ]
publishing msg "Hello From Your IoTC client!" to topic: "/devices/test-device/state"
[ custom application logic ] [ heap left: 12072 bytes ]
[ custom application logic ] [ heap left: 12072 bytes ]
[ custom application logic ] [ heap left: 12072 bytes ]
[ custom application logic ] [ heap left: 12056 bytes ]
[ custom application logic ] [ heap left: 12056 bytes ]
publishing msg "Hello From Your IoTC client!" to topic: "/devices/test-device/state"
.
.
.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

