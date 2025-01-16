Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT1170-EVKB board
- Personal Computer
- RK055MHD091 panel or RK055AHD091 panel or RK055IQH091 panel or RaspberryPi panel

Board settings
============
Connect the panel to J48. For RaspberryPi panel, connect the panel to J84,
then connect the panel's 5V pin to J85-1, GND pin to J85-2.
Connect 5V power to J43, set J38 to 1-2

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.


Steps to configure the panels
===============
The project uses RK055MHD091A0-CTG panel by default, to use other panels:

If Kconfig is used, select the panel in Kconfig "Panel Selection".

If package is used, modify the mcux_config.h:
change DEMO_PANEL to 0 to use RK055AHD091-CTG.
change DEMO_PANEL to 1 to use RK055IQH091-CTG.
change DEMO_PANEL to 2 to use RK055MHD091A0-CTG(default).
change DEMO_PANEL to 3 to use RaspberryPi panel.


Steps to configure the pixel format
===============
The project uses RGB565 pixel format by default.

When Kconfig is used
----------------
To use XRGB8888, select "Use XRGB8888 pixel format".

To use 8-bit color depth, unselect "Use XRGB8888 pixel format", select "Use LUT 8 pixel format",
then select RGB232 in "Middleware->LVGL Configuration -> Color Settings -> Color depth"

When package is used
----------------
Modify the mcux_config.h.

To use XRGB8888, change like `#define DEMO_USE_XRGB8888 1`.

To use 8-bit color depth, change like
```
#define DEMO_USE_XRGB8888 0
#define DEMO_USE_LUT8 1
#define CONFIG_LV_COLOR_DEPTH 8
#define CONFIG_LV_COLOR_DEPTH_8 1
```

Note:
1. GPU and PXP don't support 8-bit, unselect `LV_USE_DRAW_VGLITE` and `LV_USE_PXP`
in Kconfig, or modify mcux_config.h like:
```
#define CONFIG_LV_USE_DRAW_VGLITE 0
#define CONFIG_LV_USE_PXP 0
#define CONFIG_LV_ATTRIBUTE_MEM_ALIGN_SIZE 1
#define CONFIG_LV_DRAW_SW_DRAW_UNIT_CNT 1
```
2. When RGB565 or XRGB8888 is selected, changing `LV_COLOR_DEPTH` in Kconfig or mcux_config.h
doesn't affect the real pixel format.

Running the demo
===============
If this example runs correctly, the sample GUI is displayed.
