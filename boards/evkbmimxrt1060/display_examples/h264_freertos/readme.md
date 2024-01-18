Overview
========
This project read the h264 files from SD card, decode them and show the frames
in LCD panel.

Prepare the H264 files
Name the H264 files like 000.264, 001.264, 002.264, ..., and save them
to the root path of the SD card. Please starts from 000.264, if there is only one
file, name it as 000.264. This project reads the files one by one and decode.
The recommend file is: https://github.com/cisco/openh264/tree/v2.1.1/res/test_cif_P_CABAC_slice.264


SDK version
===========
- Version: 2.15.000

Toolchain supported
===================
- IAR embedded Workbench  9.40.1
- Keil MDK  5.38.1
- GCC ARM Embedded  12.2
- MCUXpresso  11.8.0

Hardware requirements
=====================
- Micro USB cable
- MIMXRT1060-EVKB board
- JLink Plus
- 5V power supply
- Personal Computer
- RK043FN02H-CT or RK043FN66HS-CT6 LCD board
  (RK043FN02H-CT and RK043FN66HS-CT6 are compatible)
- SD card

Board settings
==============
1. Connect the RK043FN02H-CT or RK043FN66HS-CT6 to board.
2. Prepare the H264 files and save in SD card.
3. Make sure resistors R368/R347/R349/R365/R363 are removed.
4. Insert the SDCARD into card slot.

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
If this example runs correctly, pictures are shown in the lcd panel.
