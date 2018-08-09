Overview
========
The SD JPEG Demo application reads the JPEG pictures from the SD card, decodes
them, and shows them in the LCD panel one by one.

Toolchain supported
===================
- Keil MDK 5.24a
- IAR embedded Workbench 8.22.2
- GCC ARM Embedded 7-2017-q4-major
- MCUXpresso10.2.0

Hardware requirements
=====================
- Micro USB cable
- EVKB-IMXRT1050 board
- JLink Plus
- 5V power supply
- Personal Computer
- RK043FN02H-CT LCD board
- SD card

Board settings
==============
1. Connect the RK043FN02H-CT board.
2. Unzip the pics.zip, place the jpg pictures to the SD card.
3. Insert the SDCARD into card slot.

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Launch the debugger in your IDE to begin running the demo.

Running the demo
================
If this example runs correctly, pictures are shown in the lcd panel one by one.

Known issue: Example built by MDK runs slow, it will be fixed in next release.
Customization options
=====================

