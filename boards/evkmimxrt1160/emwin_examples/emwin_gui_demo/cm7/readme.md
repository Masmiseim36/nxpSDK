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
- MIMXRT1160-EVK board
- Personal Computer
- RK055MHD091 panel or RK055AHD091 panel or RK055IQH091 panel

Board settings
==============
1. Connect the LCD panel to J48.

Select the panel
The project uses RK055MHD091A0-CTG panel by default, to use other panels:

If Kconfig is used, select the panel in Kconfig "Panel Selection".

If package is used, modify the mcux_config.h:
change DEMO_PANEL to 0 to use RK055AHD091-CTG.
change DEMO_PANEL to 1 to use RK055IQH091-CTG.
change DEMO_PANEL to 2 to use RK055MHD091A0-CTG(default).

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Build the project.
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
If this example runs correctly, the sample GUI is displayed.
