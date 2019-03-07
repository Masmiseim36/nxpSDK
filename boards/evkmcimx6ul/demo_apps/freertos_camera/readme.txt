Overview
========
This demo shows how to use camera, LCD, touch screen and SD card driver with
FreeRTOS.

In this demo, camera is working and captured images are displayed on LCD.
Hit the screen, the image will be captured and store to FAT FS in SD card.

Touch the screen for 1 second, the application switches to browse mode and
photos taken can be displayed on LCD. Hit the screen to switch photos.
Touch the screen for 1 second, switch back to camera mode.

Toolchain supported
===================
- IAR embedded Workbench 7.80.4
- GCC ARM Embedded 2016-5.4-q3

Hardware requirements
=====================
- Micro USB cable
- MCIMX6UL board
- JLink Plus
- 5V power supply
- Personal Computer
- LCD8000-43T LCD board
- OV5640 camera module
- SD card

Board settings
==============
1. Connect the LCD8000-43T board to J901.
2. Connect the OV5640 camera module to J1801.

Prepare the Demo
================
1.  Connect 5V power supply and JLink Plus to the board, switch SW2001 to power on the board.
2.  Connect a USB cable between the host PC and the J1901 USB port on the target board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the demo started, insert the SD card to slot. The SD card will be formated
first, this may take some time.
Customization options
=====================

