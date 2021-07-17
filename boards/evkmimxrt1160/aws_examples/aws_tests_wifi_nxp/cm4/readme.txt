Overview
========
Amazon FreeRTOS Qualification Tests

The Amazon FreeRTOS Qualification (AFQ) defines a process that the author of an Amazon FreeRTOS port must follow,
and a set of tests that the port must pass, in order for the port to be described as 'qualified by Amazon'
For more information see: <SDK_Install>/rtos/freertos/tests/Amazon FreeRTOS Qualification Developer Guide.pdf


Toolchain supported
===================
- IAR embedded Workbench  9.10.2
- Keil MDK  5.34
- GCC ARM Embedded  10.2.1

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

