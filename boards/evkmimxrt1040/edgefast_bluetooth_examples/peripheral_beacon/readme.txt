Overview
========
Application demonstrating the BLE Peripheral role, This application implements types of beacon applications .
Beacon: A simple application demonstrating the BLE Broadcaster role functionality by advertising Company Identifier, Beacon Identifier, UUID, A, B, C, RSSI.
Eddystone : The Eddystone Configuration Service runs as a GATT service on the beacon while it is connectable and allows configuration of the advertised data, the broadcast power levels, and the advertising intervals. 
iBeacon: This simple application demonstrates the BLE Broadcaster role functionality by advertising an Apple iBeacon.


Toolchain supported
===================
- MCUXpresso  11.6.0
- IAR embedded Workbench  9.30.1
- Keil MDK  5.37
- GCC ARM Embedded  10.3.1

Hardware requirements
=====================
- Micro USB cable
- evkmimxrt1040 board
- Personal Computer
- One of the following modules:
  - AzureWave AW-AM457-uSD
  - AzureWave AW-CM358-uSD
  - K32W061

Board settings
==============

Jumper settings for AzureWave AW-AM457-uSD Module:
  - J11 2-3: VIO_SD 3.3V (Voltage level of SDIO pins is 3.3V)
  - J2  1-2: 3.3V VIO_uSD (Power Supply from uSD connector)
  - J4  2-3: 3.3V VIO

The hardware should be reworked according to the hardware rework guide for evkmimxrt1040 and AW-AM457-uSD in document Hardware Rework Guide for EdgeFast BT PAL.
The pin connect for UART HCI as the following table,
------------------------------------------------------------------------------------
PIN NAME | AW-AM457-USD |   I.MXRT1040   | PIN NAME OF RT1040 | GPIO NAME OF RT1040
------------------------------------------------------------------------------------
UART_TXD |  J10(pin 4)  |   J16(pin 1)   |    LPUART3_RXD     | GPIO_AD_B1_07
UART_RXD |  J10(pin 2)  |   J16(pin 2)   |    LPUART3_TXD     | GPIO_AD_B1_06
UART_RTS |  J10(pin 6)  |   J33(pin 3)   |    LPUART3_CTS     | GPIO_AD_B1_04
UART_CTS |  J10(pin 8)  |   J33(pin 4)   |    LPUART3_RTS     | GPIO_AD_B1_05
GND      |  J6(pin 7)   |   J32(pin 7)   |    GND             | GND
------------------------------------------------------------------------------------

Jumper settings for AzureWave AW-CM358-uSD Module:
  - J2 1-2: 3.3V VIO_uSD (Power Supply from uSD connector)
  - J4 1-2: VIO 1.8V (Voltage level of SDIO pins is 1.8V)

The hardware should be reworked according to the hardware rework guide for evkmimxrt1040 and AW-CM358-uSD in document Hardware Rework Guide for EdgeFast BT PAL.
The pin connect for UART HCI as the following table,
------------------------------------------------------------------------------------
PIN NAME | AW-CM358-USD |   I.MXRT1040   | PIN NAME OF RT1040 | GPIO NAME OF RT1040
------------------------------------------------------------------------------------
UART_TXD |  J10(pin 4)  |   J16(pin 1)   |    LPUART3_RXD     | GPIO_AD_B1_07
UART_RXD |  J10(pin 2)  |   J16(pin 2)   |    LPUART3_TXD     | GPIO_AD_B1_06
UART_RTS |  J10(pin 6)  |   J33(pin 3)   |    LPUART3_CTS     | GPIO_AD_B1_04
UART_CTS |  J10(pin 8)  |   J33(pin 4)   |    LPUART3_RTS     | GPIO_AD_B1_05
GND      |  J6(pin 7)   |   J32(pin 7)   |    GND             | GND
------------------------------------------------------------------------------------

For K32W061, the readme located in <sdk>/middleware/wireless/ethermind/port/pal/mcux/bluetooth/controller/k32w061 explains how to flash the transceiver image and the hardware rework required.
The pin connect for UART HCI as the following table,
------------------------------------------------------------------------------------
PIN NAME | DK6 (K32W061) |   I.MXRT1040   | PIN NAME OF RT1040 | GPIO NAME OF RT1040
------------------------------------------------------------------------------------
UART_TXD |  PIO(pin 8)   |   J16(pin 1)   |    LPUART3_RXD     | GPIO_AD_B1_07
UART_RXD |  PIO(pin 9)   |   J16(pin 2)   |    LPUART3_TXD     | GPIO_AD_B1_06
UART_RTS |  PIO(pin 6)   |   J33(pin 3)   |    LPUART3_CTS     | GPIO_AD_B1_04
UART_CTS |  PIO(pin 7)   |   J33(pin 4)   |    LPUART3_RTS     | GPIO_AD_B1_05
------------------------------------------------------------------------------------

Note:
After downloaded binary into qspiflash and boot from qspiflash directly, 
please reset the board by pressing SW9 or power off and on the board to run the application.
Prepare the Demo
================

1.  Open example's project and build it.

2.  Connect a USB cable between the PC host and the OpenSDA USB port on the target board.

3.  Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control

4.  Download the program to the target board.

5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
Form the app_config.h file we can select witch application is starting. Default Beacon (#define BEACON_APP 1) application is starting. 
To start the Eddystone set #define EDDYSTONE 1 and others to 0. To start the iBeacon set #define IBEACON_APP 1 and others to 0.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
If BEACON_APP is 1 (and IBEACON_APP and EDDYSTONE are set to 0) the Beacon application is started.
The beacons are non-connectable advertising packets that are sent on the three advertising channels. The latter contains the following fields.
• Company Identifier (2 bytes): 0x0025 (NXP ID as defined by the Bluetooth SIG).
• Beacon Identifier (1 byte): 0xBC (Allows identifying an NXP Beacon alongside with Company Identifier).
• UUID (16 bytes): Beacon sensor unique identifier.
• A (2 bytes): Beacon application data.
• B (2 bytes): Beacon application data.
• C (2 bytes): Beacon application data.
• RSSI at 1m (1 byte): Allows distance-based applications.

Because of the hard-coded values of Beacon UUID, the application is not suitable for production use, but is quite convenient for quick demonstrations of Beacon functionality.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
If IBEACON_APP is 1 (and BEACON_APP and EDDYSTONE are set to 0) the iBeacon application is started.
This is a simple application demonstrates the BLE Broadcaster role functionality by advertising an Apple iBeacon. The calibrated RSSI @ 1 meter distance can be set using an IBEACON_RSSI build variable (e.g. IBEACON_RSSI=0xc8 for -56 dBm RSSI), or by manually editing the default value in the ibeacon.c file.

Because of the hard-coded values of iBeacon UUID, major, and minor, the application is not suitable for production use, but is quite convenient for quick demonstrations of iBeacon functionality.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
If EDDYSTONE is 1 (and IBEACON_APP and BEACON_APP are set to 0) the Eddystone application is started.

EDDYSTONE application is demonstrating Eddystone Configuration Service

The Eddystone Configuration Service runs as a GATT service on the beacon while it is connectable and allows configuration of the advertised data, the broadcast power levels, and the advertising intervals. It also forms part of the definition of how Eddystone-EID beacons are configured and registered with a trusted resolver.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
