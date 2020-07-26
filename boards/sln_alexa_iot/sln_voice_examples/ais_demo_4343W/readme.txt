Overview
========


Toolchain supported
===================
- GCC ARM Embedded  8.3.1
- MCUXpresso  11.1.0

Hardware requirements
=====================
- USB-C Cable
- SLN-ALEXA-IOT Development Kit
- Personal Computer
- SEGGER J-Link
- USB/UART converter

Board settings
==============
Ensure J27 is set to position '1' (and is not set to serial downloader mode)

Connect the board with PC by using USB/UART converter:
- board uart RX (pin 10 on J26) - connect it to TX pin on converter
- board uart TX (pin 12 on J26) - connect it to RX pin on converter
- board GND (pin 15 on J26)

Prepare the Demo
================
1. Make sure that a valid 'bootstrap' and 'bootloader' are loaded onto target board.
2. Connect a USB cable between the host PC and the USB-C port on the target board. 
3. Connect SEGGER J-Link to the 10-pin SWD connector on bottom of target board.
4. Connect USB/UART converter as described above.
5. Open a serial terminal with the USB/UART converter with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
6. Open a serial terminal with the USB-C USB CDC SHELL with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
7. Download the program to the target board.
8. Either power cycle the board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
In the USB CDC SHELL console press enter.

Enter the commands enable_usb_log and logs as shown below:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SHELL>> enable_usb_log
SHELL>> USB logging enabled
SHELL>> logs
SHELL>> 0 0 [RUN_DEMO_TASK] [INFO ][INIT][lu] SDK successfully initialized.
1 0 [APP_Task] One second delay...
2 1000 [RUN_DEMO_TASK] Initializing WiFi Connectivity ...
3 1899 [RUN_DEMO_TASK] Found no credentials in flash
4 1899 [RUN_DEMO_TASK] Failed to connect to wifi network, error 1
5 1899 [RUN_DEMO_TASK] Start AP job
6 1899 [RUN_DEMO_TASK] Access Point Start: SSID '<device-ssid>', Password '<device-password>'
7 1952 [RUN_DEMO_TASK] AP Network ready, IPv4 Address: 192.168.1.1
8 1952 [RUN_DEMO_TASK] AP started
9 1953 [UDP_Server_Task] Starting UDP server, expecting companion app messages
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Connect to the device AP point using SSID and Password for the target board.

Use the companion application to onboard the SLN-ALEXA-IOT kit with an Android or iOS device.
Customization options
=====================

