Overview
========
This project read the h264 files from SD card, decode them and show the frames
in LCD panel.

Prepare the H264 files
Name the H264 files like 000.264, 001.264, 002.264, ..., and save them
to the root path of the SD card. Please starts from 000.264, if there is only one
file, name it as 000.264. This project reads the files one by one and decode.
The recommend file is: https://github.com/cisco/openh264/blob/v2.1.1/res/test_cif_P_CABAC_slice.264


Toolchain supported
===================
- IAR embedded Workbench  9.20.2
- Keil MDK  5.36
- GCC ARM Embedded  10.3.1
- MCUXpresso  11.5.0

Hardware requirements
=====================
- Micro USB cable
- MIMXRT1170-EVK board
- JLink Plus
- 5V power supply
- Personal Computer
- RK055MHD091 panel or RK055AHD091 panel or RK055IQH091 panel
- SD card

Board settings
==============
1. Connect the RK055MHD091-CTG LCD board to J48.
2. Prepare the H264 files and save in SD card.
3. Please make sure R136 is weld for GPIO card detect.
4. Insert the SDCARD into card slot(J15).


Prepare the Demo
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Build the project, the project uses RK055MHD091 by default, to use other panels,
    change
    #define DEMO_PANEL DEMO_PANEL_RK055MHD091
    to
    #define DEMO_PANEL DEMO_PANEL_RK055IQH091
    or
    #define DEMO_PANEL DEMO_PANEL_RK055AHD091
    in display_support.h.
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
If this example runs correctly, pictures are shown in the lcd panel.
