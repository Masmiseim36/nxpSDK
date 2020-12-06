Overview
========


Toolchain supported
===================
- GCC ARM Embedded  9.2.1
- MCUXpresso  11.2.0

Hardware requirements
=====================
- USB-C Cable
- SLN-ALEXA-IOT Development Kit
- Personal Computer
- SEGGER J-Link

Board settings
==============
Ensure J27 is set to position '1' (and is not set to serial downloader mode)

Prepare the Demo
================
1. Make sure that a valid 'bootstrap' and 'bootloader' are loaded onto target board.
2. Connect a USB cable between the host PC and the OpenSDA USB port on the target board,
3. Download the program to the target board,
4. Either power cycle the board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
1. Say the default Amazon wake-word 'Alexa',
2. When detected, the RGB LED flashes blue.
Customization options
=====================

