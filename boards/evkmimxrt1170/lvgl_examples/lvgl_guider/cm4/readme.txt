Overview
========

This project is used to work with LittleVGL GUI Guider. The GUI Guider generated
code is placed in the folder "generated". The example generated code shows a
button in the panel.

Toolchain supported
===================
- IAR embedded Workbench  9.40.1
- Keil MDK  5.38.1
- GCC ARM Embedded  12.2
- MCUXpresso  11.8.0

Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT1170-EVK board
- Personal Computer
- RK055MHD091 panel or RK055AHD091 panel or RK055IQH091 panel

Board settings
==============
Connect the panel to J48
Connect 5V power to J43, set J38 to 1-2

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
    change the macro DEMO_PANEL to DEMO_PANEL_RK055IQH091 or DEMO_PANEL_RK055AHD091 in display_support.h.
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
If this example runs correctly, the sample GUI is displayed.

This board also supports LVGL 1-bit and 8-bit color depth, to use this feature, modify
DEMO_USE_XRGB8888 as 0, modify DEMO_USE_LUT8 as 1 in display_support.h. Modify
LV_COLOR_DEPTH as 1 or 8 in lv_conf.h.

Currently GPU is not supported for 1-bit and 8-bit color depth, change LV_USE_GPU_NXP_PXP
and LV_USE_GPU_NXP_VG_LITE to 0 in lv_conf.h.
