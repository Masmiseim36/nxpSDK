Overview
========
Amazon FreeRTOS Qualification Tests

The Amazon FreeRTOS Qualification (AFQ) defines a process that the author of an Amazon FreeRTOS port must follow,
and a set of tests that the port must pass, in order for the port to be described as 'qualified by Amazon'
For more information see: <SDK_Install>/rtos/freertos/tests/Amazon FreeRTOS Qualification Developer Guide.pdf

Before building the example application select Wi-Fi module macro in the app_config.h. (see #define WIFI_<SoC Name>_BOARD_<Module Name>).
For more information about Wi-Fi module connection see:
    readme_modules.txt
    Getting started guide on supported modules configuration:
    https://www.nxp.com/document/guide/getting-started-with-nxp-wi-fi-modules-using-i-mx-rt-platform:GS-WIFI-MODULES-IMXRT-PLATFORM



Toolchain supported
===================
- IAR embedded Workbench  9.20.2
- Keil MDK  5.36
- GCC ARM Embedded  10.3.1
- MCUXpresso  11.5.0

Hardware requirements
=====================
- Micro USB cable
- MIMXRT1170-EVK board
- Personal Computer


Board settings
==============

