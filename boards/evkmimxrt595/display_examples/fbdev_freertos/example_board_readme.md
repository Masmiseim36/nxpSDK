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
The demo use RK055MHD091 panel by default, to use other, change the macro DEMO_PANEL in
mcux_config.h. Which number represents which panel can be found in display_support.h.

1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

When Package is used
-------------
Modify `DEMO_PANEL` in mcux_config.h.
`#define DEMO_PANEL 0` for "MikroE TFT Proto 5" CAPACITIVE FlexIO panel.
`#define DEMO_PANEL 1` for "RK055HDMIPI4M" MIPI Rectangular panel.
`#define DEMO_PANEL 3` for "G1120B0MIPI" MIPI Circular panel.
`#define DEMO_PANEL 4` for "RK055MHD091A0-CTG MIPI Rectangular panel.(default)

Running the demo
===============
When the example runs, you can see a rectangle moving in the screen, and
its color changes when reached the border.
There is tearing effect when using the SSD1963 panel, because the panel video buffer
could not be fully updated during fresh interval.
