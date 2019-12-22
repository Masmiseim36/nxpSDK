Overview
========

The demo configures the wifi driver to use STA mode and advertises via BLE.


Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- GCC ARM Embedded  8.3.1
- MCUXpresso  11.1.0

Hardware requirements
=====================
- Micro USB cable
- evkmimxrt1020 board
- Personal Computer
- BLE enabled device such as a smartphone
- Murata 1DX M.2 module
- Murata uSD M.2 Adapter
- 6 plug to receptable header cables

Board settings
==============
Make sure C28 is welded.

Murata 1DX M.2 module is connected with Murata uSD M.2 Adapter which is plugged into uSD connector on evkmimxrt1020 board.

Murata uSD M.2 Adapter's jumper J11 is open, J1 shorts position 2-3, J12 shorts position 1-2.

The following pins between the evkmimxrt1020 board and Murata uSD M.2 Adapter are connected using the plug to receptable cables:
- evkmimxrt1020 board's connector J17, pin 3 to Murata uSD M.2 Adapter's connector J9, pin 3
- evkmimxrt1020 board's connector J17, pin 5 to Murata uSD M.2 Adapter's connector J9, pin 4
- evkmimxrt1020 board's connector J17, pin 7 to Murata uSD M.2 Adapter's connector J9, pin 2
- evkmimxrt1020 board's connector J19, pin 2 to Murata uSD M.2 Adapter's connector J9, pin 1
- evkmimxrt1020 board's connector J19, pins 4-5 to Murata uSD M.2 Adapter's connector J8, pins 3-4
Prepare the Demo
================

Running the demo
================

1. Open the project.
2. To join the test wifi network, change the SSID and password in the below macro in the source code:
    #define AP_SSID             “nxp”
    #define AP_PASS             “NXP0123456789”
3. Rebuild the application and program it to the target.
4. Once the app is booted up, scan results are printed and it joins the configured network set at step 2.
5. BLE advetising starts with the device name “mcuxpresso-hello-sensor”.
6. Download and install any of the BLE scanner apps such as LightBlue / BLE Scanner / nRF Connect to the BLE enabled device.
7. Open the app and scan for devices. Scan results shows “mcuxpresso-hello-sensor”
8. Connect to mcuxpresso-hello-sensor to view its services.

Logs:
When the application boots up the logs are as follows.


************************************************
 Wi-Fi + BLE example
************************************************
Initializing WiFi Connection...

 AsyncInterrupt is not supported
WLAN MAC Address : 00:9D:6B:8C:19:F3
WLAN Firmware    : wl0: Feb 12 2018 04:08:14 version 7.79.2 (r683798 CY) FWID 01-27b63357
WLAN CLM         : API: 12.2 Data: 9.10.39 Compiler: 1.29.4 ClmImport: 1.36.3 Creation: 2018-02-12 04:00:50
Successfully Initialized WiFi Connection
Scanning available networks...
scan completed

#001 SSID          : NXP
     BSSID         : F0:7F:06:FA:5F:6A
     RSSI          : -87dBm
     Max Data Rate : 216 Mbits/s
     Network Type  : Infrastructure
     Security      : Unknown
     Radio Band    : 2.4GHz
     Channel       : 11
Joining : nxp
Successfully joined : nxp
Getting IP address from DHCP server
 IPv4 Address got from DHCP  : 192.168.2.103

Initializing BLE...
Hello Sensor Start
hello_sensor_application_init

 wiced_bt_gatt_register: 0
wiced_bt_gatt_db_init 0
wiced_bt_cfg_settings.device_name:mcuxpresso-hello-sensor
wiced_bt_ble_set_advertisement_data 0
wiced_bt_ble_set_scan_response_data 0
Advertisement State Change: 3
wiced_bt_start_advertisements 0
Customization options
=====================

