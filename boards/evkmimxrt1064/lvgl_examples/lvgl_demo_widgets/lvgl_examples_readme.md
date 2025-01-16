Hardware requirements
=====================
- Micro USB cable
- EVK-MIMXRT1064 board
- Personal Computer
- RK043FN02H-CT or RK043FN66HS-CT6 LCD board

Board settings
============
1. Connect the RK043FN02H-CT or RK043FN66HS-CT6 to board.

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
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.running the demo.


Steps to configure the panels
===============
The project uses RK043FN66HS-CT6 panel by default, to use other panels:

If Kconfig is used, select the panel in Kconfig "Panel Selection".

If package is used, modify the mcux_config.h, change DEMO_PANEL to 0 use RK043FN02H-CT.


Steps to configure the pixel format
===============
The project uses RGB565 pixel format by default.

When Kconfig is used
----------------
Select color depth in "Middleware->LVGL Configuration -> Color Settings -> Color depth",
32-bit, 16-bit, 8-bit are supported.

When package is used
----------------
Modify the mcux_config.h change `CONFIG_LV_COLOR_DEPTH` to 32, 16, 8.

Note:
1. PXP doesn't support 8-bit, unselect `LV_USE_PXP`
in Kconfig, or modify mcux_config.h like:
```
#define CONFIG_LV_USE_PXP 0
```

Running the demo
===============
If this example runs correctly, the sample GUI is displayed.
