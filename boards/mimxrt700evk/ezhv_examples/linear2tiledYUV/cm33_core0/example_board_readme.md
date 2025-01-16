Hardware requirements
=====================
- Micro USB cable
- MIMXRT700-EVK board
- Personal Computer
- RK055AHD091 MIPI panel (Not necessary if use other panel)
- RK055MHD091 MIPI panel (Not necessary if use other panel)

Board settings
==============
To use MIPI panel:
Connect MIPI panel to J52.

Prepare the Demo
================
The demo use MIPI RK055MHD091 panel by default, to use MIPI RK055AHD091 panel, change the macro
DEMO_PANEL to DEMO_PANEL_RK055AHD091 in display_support.h. 
1.  Connect a micro USB cable between the PC host and the USB port on the board
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Launch the debugger in your IDE to begin running the demo.

Running the demo
=====================
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
When the example runs, you can see the decoded jpeg image is displayed on the  
panel of RK055AHD091 or RK055MHD091.
JPEG decode and EZHV linear2tile conversion demo start.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
