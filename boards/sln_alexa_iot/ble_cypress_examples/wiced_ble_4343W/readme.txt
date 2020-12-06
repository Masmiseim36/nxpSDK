Overview
========

The demo configures the wifi driver to use STA mode and advertises via BLE.


Toolchain supported
===================
- GCC ARM Embedded  9.2.1
- MCUXpresso  11.2.0

Hardware requirements
=====================
- USB-C Cable
- SLN-ALEXA-IOT Development Kit
- Personal Computer
- BLE enabled device such as a smartphone
- USB/UART converter
- SEGGER J-Link

Board settings
==============
Ensure J27 is set to position '1' (and is not set to serial downloader mode)

Connect the board with PC by using USB/UART converter:
- board uart RX (pin 10 on J26) - connect it to TX pin on converter
- board uart TX (pin 12 on J26) - connect it to RX pin on converter
- board GND (pin 15 on J26)

Prepare the Demo
================
1. Make sure that a valid 'bootstrap' and 'bootloader' are loaded onto target board.
2. Connect a USB cable between the host PC and the USB-C port on the target board. 
3. Connect SEGGER J-Link to the 10-pin SWD connector on bottom of target board.
3. Download the program to the target board.
4. Either power cycle the board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
Customization options
=====================

