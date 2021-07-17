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
- Micro USB cable
- MIMXRT1160-EVK board
- Personal Computer
- One of the following Wi-Fi modules:
  - Panasonic PAN9026 SDIO ADAPTER + SD to uSD adapter
  - AzureWave AW-NM191NF-uSD
  - AzureWave AW-AM457-uSD
  - AzureWave AW-CM358MA

Board settings
==============
This example, by default, is built to work with the AzureWave AW-AM457-uSD. It is configured by the macro definition in file app_config.h (#define WIFI_BOARD_AW_AM457).
If you want use the AzureWave AW-NM191NF-uSD, please change the macro to WIFI_BOARD_AW_NM191.
If you want use the Panasonic PAN9026 SDIO ADAPTER, please change the macro to WIFI_BOARD_PAN9026_SDIO.
If you want use the AzureWave AW-CM358MA, please change the macro to WIFI_BOARD_AW_CM358.

Jumper settings for AzureWave AW-NM191NF-uSD Module:
  - J11 1-2: VIO_SD 1.8V (Voltage level of SDIO pins is 1.8V)
  - J2  1-2: 3.3V VIO_uSD (Power Supply from uSD connector)

Jumper settings for AzureWave AW-AM457-uSD Module:
  - J11 1-2: VIO_SD 1.8V (Voltage level of SDIO pins is 1.8V)
  - J2  1-2: 3.3V VIO_uSD (Power Supply from uSD connector)

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
