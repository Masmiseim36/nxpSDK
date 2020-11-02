Overview
========
This is the Wi-Fi CLI example to demonstrate the CLI support usage. The CLI module allows user to add CLIs in application.
Currently only WLAN connection Manager CLIs are available.


Toolchain supported
===================
- IAR embedded Workbench  8.50.1
- GCC ARM Embedded  9.2.1
- MCUXpresso  11.2.0

Hardware requirements
=====================
- Micro USB cable
- EVK-MIMXRT1064 board
- Personal Computer
- One of the following wifi modules:
  - Panasonic PAN9026 SDIO ADAPTER + SD to uSD adapter
  - AzurWave AW-NM191MA + Murata uSD M.2 Adapter
  - AzurWave AW-NM191NF-uSD


Board settings
==============
This example is by default prepared to work with Panasonic PAN9026 SDIO ADAPTER, it is configured by project macro: WIFI_BOARD_PAN9026_SDIO.
If you want use AzurWave AW-NM191MA or AW-NM191NF change project macro WIFI_BOARD_PAN9026_SDIO to WIFI_BOARD_AW_NM191MA.

Prepare the Demo
================
1.  Connect a micro USB cable between the PC host and the CMSIS DAP USB port on the board
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Connect the WiFi module to SD card slot.
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.


Running the demo
================
1. Add CLI init API in applications main function.
2. Add WLAN CLI init API once WLAN Connection Manager gets initialized correctly.
3. When the demo starts, a welcome message would appear on the terminal, press enter for command prompt:
   Press tab or type help to list out all available CLI commands.

    wifi cli demo
    Initialize CLI
    Initialize WLAN Driver
    MAC Address: 00:13:43:91:8D:B7
    [net] Initialized TCP/IP networking stack
    WLAN is Initialized
    WLAN CLIs are initialized
    CLIs Available:

    help
    wlan-version
    wlan-mac
    wlan-scan
    wlan-scan-opt ssid <ssid> bssid ...
    wlan-add <profile_name> ssid <ssid> bssid...
    wlan-remove <profile_name>
    wlan-list
    wlan-connect <profile_name>
    wlan-start-network <profile_name>
    wlan-stop-network
    wlan-disconnect
    wlan-stat
    wlan-info
    wlan-address
    wlan-get-uap-channel
    wlan-get-uap-sta-list
    ping [-s <packet_size>] [-c <packet_count>] [-W <timeout in sec>] <ip_address>
    iperf [-s|-c <host>|-a|-h] [options]

    #
    help wlan-version wlan-mac wlan-scan wlan-scan-opt wlan-add wlan-remove wlan-list

    # help

    help 
    wlan-version 
    wlan-mac 
    wlan-scan 
    wlan-scan-opt ssid <ssid> bssid ...
    wlan-add <profile_name> ssid <ssid> bssid...
    wlan-remove <profile_name>
    wlan-list 
    wlan-connect <profile_name>
    wlan-start-network <profile_name>
    wlan-stop-network 
    wlan-disconnect 
    wlan-stat 
    wlan-info 
    wlan-address 
    wlan-get-uap-channel 
    wlan-get-uap-sta-list
    ping [-s <packet_size>] [-c <packet_count>] [-W <timeout in sec>] <ip_address>
    iperf [-s|-c <host>|-a|-h] [options]

    # wlan-version 
    WLAN Driver Version   : v1.3.r16.p1
    WLAN Firmware Version : w8977o-V2, RF87XX, FP91, 16.91.10.p83, WPA2_CVE_FIX 1, PVE1

    # wlan-mac
    MAC address
    C0:E4:34:5A:99:45

    # wlan-scan
    Scan scheduled...

    # 3 networks found:
    94:10:3E:02:60:F0  "nxp_mrvl" Infra
            channel: 1
            rssi: -25 dBm
            security: OPEN 
            WMM: YES
    94:10:3E:02:60:F1  "nxp_mrvl_5ghz" Infra
            channel: 36
            rssi: -39 dBm
            security: OPEN 
            WMM: YES
    90:72:40:21:B3:1A  "apple_g" Infra
            channel: 11
            rssi: -51 dBm
            security: WPA2 
            WMM: YES

    # wlan-scan-opt 
    Usage:
        wlan-scan-opt ssid <ssid> bssid <bssid> channel <channel> probes <probes>
    Error: invalid number of arguments

    # wlan-scan-opt ssid apple_g
    Scan for ssid "apple_g" scheduled...

    # 2 networks found:
    90:72:40:21:B3:1A  "apple_g" Infra
            channel: 11
            rssi: -52 dBm
            security: WPA2 
            WMM: YES
    90:72:40:21:B3:1B  "apple_g" Infra
            channel: 149
            rssi: -60 dBm
            security: WPA2 
            WMM: YES

    # wlan-add                  
    Usage:
    For Station interface
      For DHCP IP Address assignment:
        wlan-add <profile_name> ssid <ssid> [wpa2 <secret>]
        wlan-add <profile_name> ssid <ssid> [owe_only]
        wlan-add <profile_name> ssid <ssid> [wpa3 sae] <secret>
      For static IP address assignment:
        wlan-add <profile_name> ssid <ssid>
        ip:<ip_addr>,<gateway_ip>,<netmask>
        [bssid <bssid>] [channel <channel number>]
        [wpa2 <secret>]
    For Micro-AP interface
        wlan-add <profile_name> ssid <ssid>
        ip:<ip_addr>,<gateway_ip>,<netmask>
        role uap [bssid <bssid>]
        [channel <channelnumber>]
        [wpa2 <secret>]
    Error: invalid number of arguments

    # wlan-add abc ssid nxp_mrvl
    Added "abc"

    # wlan-connect abc
    Connecting to network...
    Use 'wlan-stat' for current connection status.

    # Connected to following BSS:SSID = [nxp_mrvl], IP = [192.168.10.152]

    # wlan-stat       
    Station connected (Active)
    uAP stopped

    # wlan-info 
    Station connected to:
    "abc"
            SSID: nxp_mrvl
            BSSID: 94:10:3E:02:60:F0 
            channel: 1
            role: Infra
            security: none

            IPv4 Address
            address: DHCP
                    IP:             192.168.10.152
                    gateway:        192.168.10.1
                    netmask:        255.255.255.0
                    dns1:           192.168.10.1
                    dns2:           0.0.0.0
    uAP not started

    # 
    # wlan-add abd ssid NXP_Soft_AP ip:192.168.10.1,192.168.10.1,255.255.255.0 role uap wpa2 12345678
    Added "abd"

    # wlan-start-network abd

    Soft AP "NXP_Soft_AP" Started successfully
    DHCP Server started successfully

    # wlan-info 
    Station connected to:
    "abc"
            SSID: nxp_mrvl
            BSSID: 94:10:3E:02:60:F0 
            channel: 1
            role: Infra
            security: none

            IPv4 Address
            address: DHCP
                    IP:             192.168.10.152
                    gateway:        192.168.10.1
                    netmask:        255.255.255.0
                    dns1:           192.168.10.1
                    dns2:           0.0.0.0
    uAP started as:
    "abd"
            SSID: NXP_Soft_AP
            BSSID: C0:E4:34:5A:99:45 
            channel: 1
            role: uAP
            security: WPA2

            IPv4 Address
            address: STATIC
                    IP:             192.168.10.1
                    gateway:        192.168.10.1
                    netmask:        255.255.255.0
                    dns1:           192.168.10.1
                    dns2:           0.0.0.0

    # 
    # wlan-disconnect 

    # Dis-connected from: nxp_mrvl
    [wlcm] Warn: got event: deauthentication

    # wlan-info 
    Station not connected
    uAP started as:
    "abd"
            SSID: NXP_Soft_AP
            BSSID: C0:E4:34:5A:99:45 
            channel: (Auto)
            role: uAP
            security: WPA2

            IPv4 Address
            address: STATIC
                    IP:             192.168.10.1
                    gateway:        192.168.10.1
                    netmask:        255.255.255.0
                    dns1:           192.168.10.1
                    dns2:           0.0.0.0

    # 

    # wlan-list     
    2 networks:
    "abc"
            SSID: nxp_mrvl
            BSSID: 00:00:00:00:00:00 
            channel: (Auto)
            role: Infra
            security: none
    "abd"
            SSID: NXP_Soft_AP
            BSSID: 00:00:00:00:00:00 
            channel: (Auto)
            role: uAP
            security: WPA2

            IPv4 Address
            address: STATIC
                    IP:             192.168.10.1
                    gateway:        192.168.10.1
                    netmask:        255.255.255.0
                    dns1:           192.168.10.1
                    dns2:           0.0.0.0

    # 

    # wlan-remove abc
    Removed "abc"

    # wlan-list 
    1 network:
    "abd"
            SSID: NXP_Soft_AP
            BSSID: 00:00:00:00:00:00 
            channel: (Auto)
            role: uAP
            security: WPA2

            IPv4 Address
            address: STATIC
                    IP:             192.168.10.1
                    gateway:        192.168.10.1
                    netmask:        255.255.255.0
                    dns1:           192.168.10.1
                    dns2:           0.0.0.0

    # 
    # 

    # wlan-address 
    not connected

    # wlan-get

    # wlan-get-uap-channel 
    uAP channel: 0

    # 


Customization options
=====================
