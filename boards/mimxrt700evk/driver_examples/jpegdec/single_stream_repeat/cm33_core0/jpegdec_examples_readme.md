Hardware requirements
=====================
- Micro USB cable
- MIMXRT700-EVK board
- Personal Computer
- RK055AHD091 panel or RK055MHD091 panel

Board settings
============
Connect the MIPI panel to MIMXRT700-EVK board J105.
For single_stream_repeat and context_switch examples, make sure JP65 1-2 are connected, and insert the
SD card which has the pictures unziped in root folder in slot J47.

Prepare the Demo
===============
1.  Connect a micro USB cable between the PC host and the CMSIS DAP USB port (J54) on the board
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Build the project, this project uses RK055MHD091 panel by default, to use use other panel,
	change the macro DEMO_PANEL in mcux_config.h. Which number represents which panel
	can be found in display_support.h.
4.  Download the program to the target board.
5.  Launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the example runs, the screen shows what described in overview.
