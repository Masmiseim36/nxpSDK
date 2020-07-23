Overview
========
Ali iot mqtt tcp demo shows basic communication with Ali IoT remote console via mqtt protocol.

Note:
The demo based on an created Aliyun account, so if you want to using your own account to run the demo,
1. Please follow the https://help.aliyun.com/product/30520.html?spm=a2c4g.11186623.6.540.145b7908BrGYVQ for getting start
2. Then create your own account and enable the Ali IoT management console, create an product.
3. Replace the ALI_IOT_PRODUCT_KEY/ALI_IOT_PRODUCT_KEY/ALI_IOT_DEVICE_NAME/ALI_IOT_DEVICE_SECRET in the ali_iot_config.h by yours
4. Rerun the demo

Toolchain supported
===================
- GCC ARM Embedded  9.2.1
- MCUXpresso  11.2.0
- IAR embedded Workbench  8.50.1
- Keil MDK  5.30

Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-KL27Z board
- Personal Computer
- SIM800C 2G module
- IoT sim card(2G)
- 5V(>=1A) power supply

Board settings
==============
1. Connect 5V(>=1A) power supply to VIN/GND
2. Connect GND of SIM800C with FRDMKL27 board GND
3. Connect pin J1-1 to SIM800C STA pin
4. Connect pin J1-3 to SIM800C EN pin
5. Connect pin J1-7 to SIM800C RXD pin
6. Connect pin J1-5 to SIM800C TXD pin

Note:
The peak current value of sim800c may reach 2A, so please be careful with VIN/GND connection, especially the power cable and power supply.

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
The log below shows the output of the hello world demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Ali IoT MQTT TCP freertos demo

mqtt_example|138 :: mqtt example

everything_state_handle|101 :: code: -0x0327, message: a1Y2qrYXwAx.iot-as-mqtt.cn-shanghai.aliyuncs.com

everything_state_handle|101 :: code: -0x0327, message: 443

everything_state_handle|101 :: code: -0x0327, message: _v=sdk-c-3.0.1,securemode=3,signmethod=hmacsha256,lan=C,_ss=1,g

everything_state_handle|101 :: code: -0x0327, message: KL27&a1Y2qrYXwAx

everything_state_handle|101 :: code: -0x0327, message: 8CBF547B24722985D299BCC7997967A38FC1AC7F039D335AE85C5FFEB0ECF30

establish tcp connection with server(host='a1Y2qrYXwAx.iot-as-mqtt.cn-shanghai.aliyuncs.com', port=[443])
success to establish tcp, fd=0
everything_state_handle|101 :: code: -0x0000, message: mqtt connected in 2311 ms

everything_state_handle|101 :: code: -0x0329, message: pub - '/ota/device/inform/a1Y2qrYXwAx/KL27': 1

everything_state_handle|101 :: code: -0x0328, message: sub - '/a1Y2qrYXwAx/KL27/user/get'

everything_state_handle|101 :: code: -0x0329, message: pub - '/a1Y2qrYXwAx/KL27/user/get': 0

example_event_handle|096 :: msg->event_type : 9

example_event_handle|096 :: msg->event_type : 3

example_message_arrive|029 :: Message Arrived:

example_message_arrive|030 :: Topic  : /a1Y2qrYXwAx/KL27/user/get

example_message_arrive|031 :: Payload: {"message":"hello!"}

example_message_arrive|032 ::
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Note:
If you rerun the demo by press the on board reset, then the case may cost more times to establish the communication with remote server.
Customization options
=====================

