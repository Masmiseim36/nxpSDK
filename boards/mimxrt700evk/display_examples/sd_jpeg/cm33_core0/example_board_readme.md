Hardware requirements
===================
- Micro USB cable
- TFT Proto 5" CAPACITIVE board HW REV 1.01 by Mikroelektronika. (Named as SSD1963 panel in project. Not necessary if use MIPI panel)
- MIMXRT700-EVK
- Personal Computer
- RK055AHD091 MIPI panel (Not necessary if use other panel)
- RK055MHD091 MIPI panel (Not necessary if use other panel)
- RaspberryPi Panel (Not necessary if use other panel)

Board settings
============
To use SSD1963 panel:
Connect SSD1963 panel to J4. Make sure to connect JP7 2&3 to use 3.3v interface.

To use MIPI panel:
Connect MIPI panel to J52.

To use Raspberry panel:
Connect the panel to J8. Then connect the panel's 5V pin to JP43-1, GND pin to JP43-2.
Make sure the R75, R76, R79, R80 are connected.

Prepare the Demo
===============
Make sure JP65 1-2 are connected, and insert the SD card which has the pictures unziped in root folder in slot J47.
The demo use MIPI RK055MHD091 panel by default, to use other panel,
change the macro DEMO_PANEL in mcux_config.h. Which number represents which panel
can be found in display_support.h.

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
If this example runs correctly, pictures are shown in the lcd panel one by one.
