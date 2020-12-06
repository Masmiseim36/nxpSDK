Overview
========


Toolchain supported
===================
- MCUXpresso  11.2.0
- GCC ARM Embedded  9.2.1

Hardware requirements
=====================
- USB-C Cable
- SLN-ALEXA-IOT Development Kit
- Personal Computer
- SEGGER J-Link
- USB/UART converter

Board settings
==============
Ensure J27 is set to position '1' (and is not set to serial downloader mode)

Connect the board with PC by using USB/UART converter:
- board uart RX (pin 10 on J26) - connect it to TX pin on converter
- board uart TX (pin 12 on J26) - connect it to RX pin on converter
- board GND (pin 15 on J26)

Prepare the Demo
================
1. Make sure that a valid 'bootstrap', 'bootloader' and 'ais_demo_4343W' (or equivalent) are loaded onto target board.
2. Connect a USB cable between the host PC and the USB-C port on the target board. 
3. Connect SEGGER J-Link to the 10-pin SWD connector on bottom of target board. (Optional)
4. Connect USB/UART converter as described above.
5. Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
6. Either power cycle the board or launch the debugger in your IDE to begin running the demo.
7. Make sure Ivaldi_sln_alexa_iot package is available on host machine.

Running the demo
================

Follow directions in the "Updater bundle generate script" section of Ivaldi_sln_alexa_iot/Scripts/ota_signing/README.md.
Customization options
=====================

