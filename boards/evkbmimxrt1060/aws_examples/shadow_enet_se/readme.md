Overview
========
IoT Reference Integration on NXP i.MX RT1060 MCU and EdgeLock® SE050 Secure Element.
A simple shadow demo that updates the device’s powerOn state and runs concurrently with the over-the-air firmware update background task.



SDK version
===========
- Version: 2.15.000

Toolchain supported
===================
- IAR embedded Workbench  9.40.1
- Keil MDK  5.38.1
- GCC ARM Embedded  12.2
- MCUXpresso  11.8.0

Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT1060-EVKB board
- Personal Computer
- Network cable RJ45 standard (Network with Internet access)
- OM-SE050ARD-E

Board settings
==============
    -J14 3-4 (Default): EdgeLock SE05x_VCC pin routed to SE05X_VOUT pin
    -J15 3-4 (Default): I2C target SDA connection
    -J17 3-4 (Default): I2C target SCL connection

Prepare the Demo
================
The demo requires MCUBoot bootloader to be present in the FLASH memory to function properly.
It is recommended to build and program the bootloader first, then go on with the application.
Please refer to respective readme of the mcuboot_opensource example and follow the steps there before you continue.

For comprehensive guide on seeting up AWS OTA, please see "AWS OTA User Guide.pdf" document.

Steps to run demo:
1.  Download the program to the target board with the CMSIS-DAP or J-Link debugger.

2.  Provision Device and Setup AWS Account
    https://github.com/FreeRTOS/iot-reference-nxp-rt1060/blob/main/GSG.md#4-provision-device-and-setup-aws-account
    NOTE:
      CLI commands use different ID in this example:

        To get the pre-provisioned X.509 certificate from the secure element:
          pki get cert sss:010100F0

        Provisioning the Application Code Signing Key to the Device:
          pki set pub_key sss:44332200


3.  Set macro appmainPROVISIONING_MODE to 0 in app_main.c (located in project folder) and rebuild project.

4.  Download the program again to the target board with the CMSIS-DAP or J-Link debugger.

5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

6.  Open a serial terminal on PC for serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control


Running the demo
================
To get the application properly executed by the bootloader, it is necessary to put signed application image to the primary application partition.
There are multiple options how to achieve that, however in principle the are two main methods (both presuming the bootlaoder is already in place):

a) programing signed application image to the primary application partition using an external tool (direct method)
b) jump-starting the application by debugger, performing OTA update with the signed image, resetting the board and letting the bootloader to perform update (indirect method)

The latter method is described step by step below:

1.  Load the demo project and build it.
    Known issue: MDK linker issues warning about unused boot_hdr sections. This does not affect the functionality of the example.
    
2.  Prepare signed image of the application from raw binary as described in the mcuboot_opensource readme.
     - In case of MCUXpress raw binary may not be generated automatically. Use binary tools after right clicking Binaries/.axf file in the project tree to generate it manually.
    
3.  Launch the debugger in your IDE to jump-start the application.
     - In case of MCUXpresso IDE the execution stalls in an endless loop in the bootloader. Pause the debugging and use debugger console and issue command 'jump ResetISR'.

4.  The OTA task connects to the cloud service and expects an update package to be deployed.
    Plese refer to https://docs.aws.amazon.com/freertos/latest/userguide/ota-manager.html on how to create an update job with the signed image.
    Note: the image signed for MCUBoot (using imgtool) needs to be further signed for AWS OTA job. These are distinct signatures that are being checked at different stages of the OTA/boot process.
    
5.  After the OTA agent (that is part the the demo) gets notification about the update it starts downloading the image and storing it into secondary application partition.

6.  Once the image is downloaded, bootloader is notified about it and reboot in test-mode takes place.
    If the updated firmware is able to run without issues the update is made permanent automatically.




The log below shows the output of the demo in the terminal window. The log can be different based on your local network configuration.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
.
.
.
Initializing PHY...
PHY Auto-negotiation failed. Please check the cable connection and link partner setting.
0 9001 [Tmr Svc] Getting IP address from DHCP ...

1 9679 [Tmr Svc] IPv4 Address: 192.168.1.109

2 9679 [Tmr Svc] DHCP OK

3 9681 [MQTT] [INFO] Creating a TLS connection to a2zcot8a2tqh6c-ats.iot.us-east-2.amazonaws.com:8883.
4 9682 [OTA] [INFO] OTA over MQTT demo, Application version 0.9.3
5 9684 [OTAAgent] [INFO] Current State=[RequestingJob], Event=[Start], New state=[RequestingJob]
6 9684 [OTA] [INFO]  Received: 0   Queued: 0   Processed: 0   Dropped: 0
-6s:smCom:Invalid conn_ctx
-6s:sss:INFO  (Len=1610877964)
		 0 A0  0  0 	 3 96  4  3 	E8  0 FE  2 	 B  3 E8  8 
		 1  0  0  0 	 0 64  0  0 	 A 4A 43 4F 	50 34 20 41 
		54 50 4F 
-6s:sss:Communication channel is Plain.
-6s:sss:!!!Not recommended for production use.!!!
7 11807 [MQTT] [INFO] (Network connection 2023ba28) TLS handshake successful.
8 11807 [MQTT] [INFO] (Network connection 2023ba28) Connection to a2zcot8a2tqh6c-ats.iot.us-east-2.amazonaws.com established.
9 11807 [MQTT] [INFO] Creating an MQTT connection to the broker.
10 12163 [MQTT] [INFO] MQTT connection established with the broker.
11 12164 [MQTT] [INFO] Successfully connected to MQTT broker.
12 12172 [SHADOW_DEV] [INFO] MQTT Agent is connected. Initializing shadow device task.
13 12173 [SHADOW_APP] [INFO] MQTT Agent is connected. Initializing shadow update task.
14 12173 [SHADOW_APP] [INFO] Sending subscribe request to agent for shadow topics.
15 12187 [SHADOW_DEV] [INFO] Sending subscribe request to agent for shadow topics.
16 12469 [OTAAgent] [INFO] Subscribed to topic $aws/things/aws_rpr_michal/jobs/notify-next.


17 12469 [OTAAgent] [INFO] Subscribed to MQTT topic: $aws/things/aws_rpr_michal/jobs/notify-next
18 12469 [MQTT] [INFO] Publishing message to $aws/things/aws_rpr_michal/jobs/$next/get.

19 12479 [SHADOW_APP] [INFO] Received subscribe ack for shadow update topics.
20 12530 [SHADOW_DEV] [INFO] Successfully subscribed to shadow update topics.
21 12531 [SHADOW_DEV] [INFO] Publishing to /get message using client token 12531.
22 12531 [MQTT] [INFO] Publishing message to $aws/things/aws_rpr_michal/shadow/get.

23 12537 [SHADOW_DEV] [INFO] Successfully sent a publish message to /get topic.
24 12738 [MQTT] [INFO] Ack packet deserialized with result: MQTTSuccess.
25 12738 [MQTT] [INFO] State record updated. New state=MQTTPublishDone.
26 12740 [MQTT] [INFO] Ack packet deserialized with result: MQTTSuccess.
27 12740 [MQTT] [INFO] State record updated. New state=MQTTPublishDone.
28 12742 [OTAAgent] [INFO] Sent PUBLISH packet to broker $aws/things/aws_rpr_michal/jobs/$next/get to broker.


29 12742 [OTAAgent] [WARN] OTA Timer handle NULL for Timerid=0, can't stop.
31 12747 [MQTT] [INFO] De-serialized incoming PUBLISH packet: DeserializerResult=MQTTSuccess.
30 12742 [OTAAgent] [INFO] Current State=[WaitingForJob], Event=[RequestJobDocument], New state=[WaitingForJob]
32 12747 [MQTT] [INFO] State record updated. New state=MQTTPublishDone.
33 12799 [MQTT] [INFO] Received OTA job message, size: 56.


34 12802 [OTAAgent] [INFO] No active job available in received job document: OtaJobParseErr_t=OtaJobParseErrNoActiveJobs
35 12802 [OTAAgent] [WARN] Received an unhandled callback event from OTA Agent, event = 8
36 12802 [OTAAgent] [INFO] Current State=[WaitingForJob], Event=[ReceivedJobDocument], New state=[CreatingFile]
37 12812 [MQTT] [INFO] De-serialized incoming PUBLISH packet: DeserializerResult=MQTTSuccess.
38 12812 [MQTT] [INFO] State record updated. New state=MQTTPubAckSend.
39 12847 [MQTT] [INFO] Ignoring GET accepted response without a delta field.
40 12850 [SHADOW_DEV] [INFO] Received an accepted response for shadow GET request. 
41 14685 [OTA] [INFO]  Received: 0   Queued: 0   Processed: 0   Dropped: 0
42 17479 [SHADOW_APP] [INFO] Publishing to /update with following client token 17479.
43 17479 [MQTT] [INFO] Publishing message to $aws/things/aws_rpr_michal/shadow/update.

44 17790 [MQTT] [INFO] Ack packet deserialized with result: MQTTSuccess.
45 17791 [MQTT] [INFO] State record updated. New state=MQTTPublishDone.
46 17799 [MQTT] [INFO] De-serialized incoming PUBLISH packet: DeserializerResult=MQTTSuccess.
47 17799 [MQTT] [INFO] State record updated. New state=MQTTPubAckSend.
48 17800 [MQTT] [INFO] Received delta update with version 1678.
49 17800 [MQTT] [INFO] Setting device state to 0.
50 17812 [MQTT] [INFO] De-serialized incoming PUBLISH packet: DeserializerResult=MQTTSuccess.
51 17813 [MQTT] [INFO] State record updated. New state=MQTTPubAckSend.
52 17813 [MQTT] [INFO] Received accepted response for update with token 17479. 
53 17814 [SHADOW_DEV] [INFO] Publishing to /update with following client token 17813.
54 17850 [MQTT] [INFO] Publishing message to $aws/things/aws_rpr_michal/shadow/update.

55 17858 [SHADOW_DEV] [INFO] Successfully sent a publish message to /update topic.
56 18166 [MQTT] [INFO] De-serialized incoming PUBLISH packet: DeserializerResult=MQTTSuccess.
57 18166 [MQTT] [INFO] State record updated. New state=MQTTPubAckSend.
58 18167 [MQTT] [INFO] Received accepted response for update with token 17813. 
59 18170 [SHADOW_DEV] [INFO] Successfully received a shadow update accepted message from cloud. 
60 19685 [OTA] [INFO]  Received: 0   Queued: 0   Processed: 0   Dropped: 0
61 22845 [SHADOW_APP] [INFO] Publishing to /update with following client token 22845.
62 22845 [MQTT] [INFO] Publishing message to $aws/things/aws_rpr_michal/shadow/update.

63 23256 [MQTT] [INFO] Ack packet deserialized with result: MQTTSuccess.
64 23256 [MQTT] [INFO] State record updated. New state=MQTTPublishDone.
65 23314 [MQTT] [INFO] De-serialized incoming PUBLISH packet: DeserializerResult=MQTTSuccess.
66 23315 [MQTT] [INFO] State record updated. New state=MQTTPubAckSend.
67 23315 [MQTT] [INFO] Received delta update with version 1680.
68 23315 [MQTT] [INFO] Setting device state to 1.
69 23327 [MQTT] [INFO] De-serialized incoming PUBLISH packet: DeserializerResult=MQTTSuccess.
70 23327 [MQTT] [INFO] State record updated. New state=MQTTPubAckSend.
71 23328 [MQTT] [INFO] Received accepted response for update with token 22845. 
72 23331 [SHADOW_DEV] [INFO] Publishing to /update with following client token 23331.
73 23361 [MQTT] [INFO] Publishing message to $aws/things/aws_rpr_michal/shadow/update.
.
.
.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
