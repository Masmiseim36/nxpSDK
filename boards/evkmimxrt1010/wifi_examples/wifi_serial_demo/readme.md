Overview
========
The wifi_serial_demo application demonstrates usage of SerialMWM Wi-Fi module connected over UART interface.

It provides command menu for access to actions in following categories:
 - WLAN operation/configuration
 - WLAN firmware upgrade
 - Client socket communication (TCP/UDP)

Description of menu items:
0 - WLAN State
    Prints information about current state of the WLAN module.

1 - WLAN Scan
    Prints cached Wi-Fi scan results and schedules a new scan.
 
2 - WLAN Connect
    Attempts to connect Wi-Fi module with the configured WLAN (see menu item 4 for configuration options). Command applicable in disconnected state only.

3 - WLAN Disconnect
    Disconnects WLAN module from a WLAN it is connected to.

4 - WLAN Configuration - Client
    Demonstrates configuration of WLAN for connection.
    The application prints information about current configuration and asks if you want to change it.
    In case of change it will ask you for input of new WLAN parameters (SSID, security, passphrase).
    After successful configuration the application reboot the WLAN module to apply changes.

5 - WLAN Configuration - Provisioning mode
    When the SerialMWM is in the provisioning mode, it hosts a WLAN network (micro-AP) for the provisioning.
    Default configuration of WLAN in the provisioning mode is:
        SSID: Serial2Wifi
        Password: nxp12345
    This command demonstrates configuration of this micro-AP network parameters (SSID, security, passphrase).
    The application prints information about current configuration and asks if you want to change it.
    In case of change it will ask you for input of new parameters (SSID, security, passphrase).
    After successful configuration the application reboot the WLAN module to apply changes.
 
6 - WLAN Reset to provisioning mode
    Resets the WLAN module to provisioning mode.
 
7 - WLAN Reboot
    Reboots the SerialMWM device. The SerialMWM is rebooted irrespective of the state of the device.
 
8 - SerialMWM FW Upgrade
    Performs upgrade of the SerialMWM firmware. For the firmware upgrade is necessary prepare PC with HTTP server providing the firmware file (see section below).
    Application will ask you for IP address and port of the HTTP server and then will ask you to select the firmware file for upgrade.
    After a successful upgrade, the application reboots SerialMWM to use the upgraded firmware. In case the upgrade fails, the old firmware image continues to be in use.

9 - WLAN FW Upgrade
    Performs upgrade of the WLAN firmware. For the firmware upgrade is necessary prepare PC with HTTP server providing the firmware file (see section below).
    Application will ask you for IP address and port of the HTTP server and then will ask you to select the firmware file for upgrade.
    After a successful upgrade, the application reboots SerialMWM to use the upgraded firmware. In case the upgrade fails, the old firmware image continues to be in use.
 
10 - TCP Socket test (Client)
    Demonstrates communication over TCP socket as client. It creates socket, connects to server, sends data, receives data and closes socket.
    It is necessary to follow instructions from the application and start TCP Echo server (see section below).
    Application will ask you for IP address and port of the TCP Echo server.

11 - UDP Socket test
    Demonstrates communication over UDP socket. It creates socket, sends data, receives data and closes socket.
    It is necessary to follow instructions from the application and start UDP Echo server (see section below).
    Application will ask you for IP address and port of the UDP Echo server.


HTTP Server
The SerialMWM module uses HTTP protocol for firmware file transfer during firmware upgrade (menu items 8 and 9).
As a simple HTTP server providing firmware file can be used Python 3. Call this CMD from the directory of the firmware file:
    python -m http.server 8000
It starts HTTP server listening on port 8000. This server provides list of files from directory from which was started. It is used in this application, which allows you select file for upgrade in the terminal.
Please make sure the IP address is reachable from the SerialMWM module (start the HTTP server on PC in the same network, which is connected WLAN module).


Echo Server
As TCP/UDP Echo server can be used EchoTool: https://github.com/PavelBansky/EchoTool
It can be downloaded from: https://github.com/pbansky/EchoTool/releases/download/v1.5.0.0/echotool.exe
Please start the Echo server on PC in the same network, which is connected WLAN module.

For TCP Echo server call from CMD:
    echotool /p tcp /s 7
It starts TCP echo server listening on port 7.

For UDP Echo server call from CMD:
    echotool /p udp /s 7
It starts UDP echo server on port 7.


More information about the SerialMWM Wi-Fi module and the UART communication protocol can be found in the SerialMWM API Reference Manual in Wi-Fi documentation.



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
- Two Micro USB cable
- EVK-MIMXRT1010 board
- Personal Computer
- Murata CMWC1ZZABR-107-EVB
- LPCXpresso Click Shield

Board settings
==============
Connect the LPCXpresso Click Shield to the board and then connect the Murata Wi-Fi module to mikroBUS slot 2.
Debug console is configured as virtual COM - please connect a micro USB cable between the PC host and USB OTG (J9).

Jumper settings:
J31: open
J32: open


Note:
To debug in qspiflash, following steps are needed:
1. Select the flash target and compile.
2. Set the SW8: 1 off 2 off 3 on 4 off, then power on the board and connect USB cable to J41.
3. Start debugging in IDE.
   - Keil: Click "Download (F8)" to program the image to qspiflash first then clicking "Start/Stop Debug Session (Ctrl+F5)" to start debugging.

Note:
When the USB serial port is emuerated and termial is opened, the printf has been executed, so the message cannot be displayed.
Please use putty,sscom and other terminal that support data caching replace it.

Prepare the Demo
================
1.  Connect a USB cable between the PC host and the OpenSDA(or USB to Serial) USB port on the target board.
2.  Open a serial terminal on PC for OpenSDA serial(or USB to Serial) device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.


Running the demo
================
The log below shows the output of the demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Serial Wi-Fi Demo

Initializing...

SerialMWM FW Version: 3.1
WLAN FW Version: SD878x-14.88.36.p144-702.1.0-WM

Starting WLAN...

WLAN State: Connecting to NXP


-------- MENU --------
 0 - WLAN State
 1 - WLAN Scan
 2 - WLAN Connect
 3 - WLAN Disconnect
 4 - WLAN Configuration - Client
 5 - WLAN Configuration - Provisioning mode
 6 - WLAN Reset to provisioning mode
 7 - WLAN Reboot
 8 - SerialMWM FW Upgrade
 9 - WLAN FW Upgrade
10 - TCP Socket test
11 - UDP Socket test
Select from menu: 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

