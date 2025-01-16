Hardware requirements
===================
- Micro USB cable
- TFT Proto 5" CAPACITIVE board HW REV 1.01 by Mikroelektronika. (Named as SSD1963 panel in project. Not necessary if use MIPI panel)
- EVK-MIMXRT595
- Personal Computer
- RK055AHD091 MIPI panel (Not necessary if use other panel)
- RK055MHD091 MIPI panel (Not necessary if use other panel)
- RM67162 smart MIPI panel (Not necessary if use other panel)

Board settings
============
To use SSD1963 panel:
Connect SSD1963 panel to J43.

To use MIPI panel:
Connect MIPI panel to J44.

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
The demo use MIPI RK055MHD091 panel by default.
If Kconfig is used, select the panel in Kconfig "Panel Selection".
To use SSD1963 DBI panel, change the macro DEMO_PANEL to 0 in mcux_config.h.
To use MIPI RK055AHD091 panel, change the macro DEMO_PANEL to 1 in mcux_config.h.
To use MIPI RK055IQH091 panel, change the macro DEMO_PANEL to 2 in mcux_config.h.
To use RM67162 MIPI smart DBI panel, change the macro DEMO_PANEL to 3 in mcux_config.h.
To use MIPI RK055MHD091 panel, change the macro DEMO_PANEL to 4 in mcux_config.h(default).

Running the demo
===============
When the example runs, you can see a polygon vector graphic with high render quality on blue buffer.
