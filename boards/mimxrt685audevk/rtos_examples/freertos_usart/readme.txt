Overview
========
The USART example for FreeRTOS demonstrates the possibility to use the USART driver in the RTOS.
The example uses single instance of USART IP and writes string into, then reads back chars.
After every 4B received, these are sent back on USART.

Toolchain supported
===================
- IAR embedded Workbench  9.32.1
- Keil MDK  5.37
- GCC ARM Embedded  10.3.1
- MCUXpresso  11.7.0

Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT685-AUD-EVK board
- Personal Computer

Board settings
==============
This example project does not call for any special hardware configurations.
Although not required, the recommendation is to leave the development board's jumper settings
and configurations in default state when running this example.
