Overview
========
Amazon FreeRTOS Qualification Tests

The Amazon FreeRTOS Qualification (AFQ) defines a process that the author of an Amazon FreeRTOS port must follow,
and a set of tests that the port must pass, in order for the port to be described as 'qualified by Amazon'
For more information see: <SDK_Install>/rtos/freertos/tests/Amazon FreeRTOS Qualification Developer Guide.pdf


Toolchain supported
===================
- IAR embedded Workbench  8.50.1
- GCC ARM Embedded  9.2.1
- MCUXpresso  11.2.0

Hardware requirements
=====================
- Micro USB cable
- evkmimxrt1060 board
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

Running the demo
================

Customization options
=====================

