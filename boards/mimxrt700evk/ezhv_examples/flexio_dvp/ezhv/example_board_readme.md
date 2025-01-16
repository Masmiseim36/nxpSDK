Hardware requirements
=====================
- Micro USB cable
- MIMXRT700-EVK board
- Personal Computer
- RK055AHD091 MIPI panel (Not necessary if use other panel)
- RK055MHD091 MIPI panel (Not necessary if use other panel)
- OV7670 module
Board settings
============ 
To use MIPI panel:
Connect MIPI panel to J52.

Prepare the Demo
===============
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

Running the demo CM33
=====================
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
When the demo runs successfully, you will find the video is dislayed on the  
panel of RK055AHD091 or RK055MHD091.
and the terminal will display the following:
    EZH-V flexIO DVP example started
    Frame Update Rate: 28 fps
    Frame Update Rate: 28 fps
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Note:
=====================
Connect the OV7670 camera sensor to J53 connector
short 2-3 for JP7 
