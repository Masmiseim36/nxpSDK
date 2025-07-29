Hardware requirements
===================
- Micro USB cable
- MIMXRT700-EVK
- Personal Computer
- RK055AHD091 MIPI panel (Not necessary if use other panel)
- RK055MHD091 MIPI panel (Not necessary if use other panel)
- RaspberryPi Panel (Not necessary if use other panel)

Board settings
============
Connect JP1 and JP3.

To use MIPI panel:
Connect MIPI panel to J52.

To use Raspberry panel:
Connect the panel to J8. Then connect the panel's 5V pin to JP43-1, GND pin to JP43-2.
Make sure the R75, R76, R79, R80 are connected.

This demo does not support other panels.

Prepare the Demo
===============
The demo use MIPI RK055MHD091 panel by default, to use other panel,
set the coresponding panel DEMO_PANEL_xxx(without '_SUPPORT') to y in prj.conf under board example folder before project generation.
The supported panels can be found in examples\_boards\<board>\project_segments\display_support\Kconfig.prjseg.
Or change the macro DEMO_PANEL in mcux_config.h of the project,
which number represents which panel can be found in display_support.h

1.  Connect a USB cable between the host PC and the MCU-LINK USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
===============
When the example runs, you can see the picture displayed in the up left corner of the panel.
