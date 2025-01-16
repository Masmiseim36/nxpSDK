Overview
========
The example demonstrates graphical widgets of the emWin library.


SDK version
===========
- Version: 24.12.00

Toolchain supported
===================
- Keil MDK  5.41
- IAR embedded Workbench  9.60.3
- GCC ARM Embedded  13.2.1
- MCUXpresso  24.12.00

Hardware requirements
=====================
- Micro USB cable
- TFT Proto 5" CAPACITIVE board HW REV 1.01 by Mikroelektronika. (Named as SSD1963 panel in project. Not necessary if use MIPI panel)
- EVK-MIMXRT595
- Personal Computer
- RK055AHD091 MIPI panel (Not necessary if use other panel)
- RK055MHD091 MIPI panel (Not necessary if use other panel)
- RM67162 smart MIPI panel (Not necessary if use other panel)

Board settings
==============
To use SSD1963 panel:
Connect SSD1963 panel to J43. Touch capability is enabled.

To use MIPI panel:
Connect MIPI panel to J44. Touch capability is enabled.

Select the panel
SSD1963 panel is used by default, follow the steps to configure the sw to support different panel.

When Kconfig is used
-------------
Select the panel in Kconfig "Panel Selection".

When Package is used
-------------
Modify `DEMO_PANEL` in mcux_config.h.
`#define DEMO_PANEL 0` for "MikroE TFT Proto 5" CAPACITIVE FlexIO panel(default).
`#define DEMO_PANEL 1` for "RK055HDMIPI4M" MIPI Rectangular panel.
`#define DEMO_PANEL 3` for "G1120B0MIPI" MIPI Circular panel.
`#define DEMO_PANEL 4` for "RK055MHD091A0-CTG MIPI Rectangular panel.

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
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
If this example runs correctly, the sample GUI is displayed.
NOTE: As the example is designed for rectangle display, if you use RM67162 round panel, the content at the
corners will not be seen.
