Overview
========
Amazon FreeRTOS Qualification Tests

The Amazon FreeRTOS Qualification (AFQ) defines a process that the author of an Amazon FreeRTOS port must follow,
and a set of tests that the port must pass, in order for the port to be described as 'qualified by Amazon'
For more information see: <SDK_Install>/rtos/amazon-freertos/tests/Amazon FreeRTOS Qualification Developer Guide.pdf


Toolchain supported
===================
- Keil MDK  5.29
- GCC ARM Embedded  8.3.1
- IAR embedded Workbench  8.40.2
- MCUXpresso  11.1.0

Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-K64F board
- "GT202 Adaptor V1.04" or "Silex SX-ULPAN-2401" or "FRDM-KL64Z click shield with WIFI10CLICK"
- Personal Computer

Board settings
==============
"Plug SX-ULPAN-2401" or "GT202 Adaptor" or "WIFI10CLICK" board to FRDM stackable headers (J1, J2, J3, J4).
GT202 shield is compatible with FRDM-K64 SCH-REV E and higher.
Previous revisions cannot provide GPIO mux on J2:2 (WLAN_PWRON).

The default compilation settings are predefined for WIFI10CLICK shield.
If you want to use "GT202 shield" or "Silex SX-ULPAN-2401", please add following "Compiler define" in project options:

WIFISHIELD_IS=WIFISHIELD_IS_GT202
WIFISHIELD_IS=WIFISHIELD_IS_SILEX2401

The "wifi_shield.h" then includes specific shield support.
Prepare the Demo
================

Running the demo
================

Customization options
=====================

