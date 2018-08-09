Overview
========
The fee_bple example shows how to use MCG driver to change from FEE mode to BLPE mode:

 1. How to use the mode functions for MCG mode switch.
 2. How to use the frequency functions to get current MCG frequency.
 3. Work flow
    Boot to Fee mode from default reset mode
    Change from FEE -> FBE -> BLPE
    Change back BLPE -> FBE -> FEE
    Get System clock in FEE mode to blink LED

In this example, because the debug console's clock frequency may change,
so the example running information is not output from debug console. Here the
LED blinks to show that the example finished successfully.

Toolchain supported
===================
- IAR embedded Workbench 7.80.4
- Keil MDK 5.21a
- GCC ARM Embedded 2016-5.4-q3
- Kinetis Development Studio IDE 3.2.0
- MCUXpresso0.8

Hardware requirements
=====================
- Mini/micro USB cable
- USB-KW41Z board
- Personal Computer

Board settings
==============
This example project does not call for any special hardware configurations.
Although not required, the recommendation is to leave the development board's jumper settings
and configurations in default state when running this example.
Prepare the Demo
================

Running the demo
================

Customization options
=====================

