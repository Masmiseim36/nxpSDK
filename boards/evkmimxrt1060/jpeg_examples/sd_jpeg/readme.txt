Overview
========
The SD JPEG Demo application reads the JPEG pictures from the SD card, decodes
them, and shows them in the LCD panel one by one.
Note: Please make sure the card you are using is FAT32 format.

Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- Keil MDK  5.29
- GCC ARM Embedded  8.3.1
- MCUXpresso  11.1.0

Hardware requirements
=====================
- Micro USB cable
- EVK-MIMXRT1060 board
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
3.	Insert SD card to card slot 
4.  Download the program to the target board.
5.  Launch the debugger in your IDE to begin running the demo.

Running the demo
================
If this example runs correctly, pictures are shown in the lcd panel one by one.
Customization options
=====================

