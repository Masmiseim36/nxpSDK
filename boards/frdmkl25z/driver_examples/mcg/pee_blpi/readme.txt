Overview
========
The pee_bpli example shows how to use MCG driver to change from PEE mode to BLPI mode:

 1. How to use the mode functions for MCG mode switch.
 2. How to use the frequency functions to get current MCG frequency.
 3. Work flow
    Boot to PEE mode from default reset mode
    Change mode PEE -> PBE ->FBE -> FBI -> BLPI
    Change back BLPI -> FBI -> FBE -> PBE -> PEE
    Get System clock in PEE mode to blink LED

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
- Mini/Micro USB cable
- FRDM-KL25Z board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
1. Connect a mini/micro USB cable between the PC host and the OpenSDA USB port on FRDMKL25Z board.
2. Download the program to the target board.
3. Either press the reset button on your board or launch the debugger in your IDE to begin running
   the demo. For detailed instructions, see Getting Started with Software Development Kit for
   KL25Z.

Running the demo
================

When the example runs successfully, the board's LED will blink.
Customization options
=====================

