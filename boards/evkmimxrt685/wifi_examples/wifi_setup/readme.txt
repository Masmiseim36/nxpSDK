Overview
========
This is a simpe wifi setup demo.


Toolchain supported
===================
- IAR embedded Workbench  9.10.2
- Keil MDK  5.34
- GCC ARM Embedded  10.2.1
- MCUXpresso  11.4.0

Hardware requirements
=====================
- Mini/micro USB cable
- EVK-MIMXRT685 board
- Personal Computer
- One of the following Wi-Fi modules:
  - Panasonic PAN9026 SDIO ADAPTER
  - AzureWave AW-NM191NF-uSD + uSD to SD adapter
  - AzureWave AW-AM457-uSD + uSD to SD adapter
  - AzureWave AW-CM358-uSD + uSD to SD adapter
  - Murata Type 2DS + Murata uSD-M.2 adapter + uSD to SD adapter
  - Murata Type 1XK + Murata uSD-M.2 adapter + uSD to SD adapter
  - Murata Type 1ZM + Murata uSD-M.2 adapter + uSD to SD adapter


Board settings
==============
Before building the example application select Wi-Fi module macro in the app_config.h. (see #define WIFI_<SoC Name>_BOARD_<Module Name>)

Getting started guide on supported modules configuration:
https://www.nxp.com/document/guide/getting-started-with-nxp-wi-fi-modules-using-i-mx-rt-platform:GS-WIFI-MODULES-IMXRT-PLATFORM


Jumper settings for AzureWave AW-NM191NF-uSD Module:
  - J11 1-2: VIO_SD 1.8V (Voltage level of SDIO pins is 1.8V)
  - J2  1-2: 3.3V VIO_uSD (Power Supply from uSD connector)

Jumper settings for AzureWave AW-AM457-uSD Module:
  - J11 1-2: VIO_SD 1.8V (Voltage level of SDIO pins is 1.8V)
  - J2  1-2: 3.3V VIO_uSD (Power Supply from uSD connector)

Jumper settings for AzureWave AW-CM358-uSD Module:
  - J4 1-2: VIO 1.8V (Voltage level of SDIO pins is 1.8V)
  - J2 1-2: 3.3V VIO_uSD (Power Supply from uSD connector)
  - The pin 1 of J4 is not marked on the board. Please note that pin numbering of J4 is opposite to J2 (pin 1 is close to the "J4" label):
         3 2 1
         o o=o J4
      J2 o=o o
         1 2 3

Jumper settings for Murata uSD-M.2 adapter:
  - Both J12 & J13 = 1-2: WLAN-SDIO = 1.8V; and Optional WLAN/BT-CTRL & BT-UART = 3.3V
  - J1 = 2-3: 3.3V from uSD connector


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
1. Adjust the SSID and PASSPHRASE definition to fit your wifi network and adjust the ping address definitions.
2. After wifi is initialized, it'll try to scan nearby networks. After the scan results appear press any key to continue.
3. Wifi will now try to connect to the defined network. After that you'll be one more time prompted to press any key.
