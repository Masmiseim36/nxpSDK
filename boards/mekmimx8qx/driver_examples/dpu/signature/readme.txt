Overview
========

This project shows how to use the DPU signature unit. It shows three rectangles,
the blue one and yellow one indicate two signature evaluation windows. The red
rectangle is moving in the screen, when the red rectangle moved into the regions
of evaluation window, the window's measured CRC is different with the reference
value, then panic occurs, the window turns to black accordingly.

Toolchain supported
===================
- IAR embedded Workbench  8.32.1
- GCC ARM Embedded  7.3.1

Hardware requirements
=====================
- Micro USB cable
- MIMX8QX MEK board
- J-Link Debug Probe
- 12V power supply
- MX8_DSI_OLED1 board
- Personal Computer

Another option is using monitor but not OLED panel, in this case MX8_DSI_OLED1 board
is not required, LVDS or MIPI_DSI could be used. To use LVDS, these are required:
- X-IMX-LVDS-HDMI convert board
- Monitor supports HDMI input
To use MIPI_DSI, these are required:
- X-IMX-MIPI-HDMI convert board
- Monitor supports HDMI input

Board settings
==============
If use MX8_DSI_OLED1, connect J1 and MX8_DSI_OLED1 board.
If use monitor to display through MIPI DSI, connect J1 and X-IMX-MIPI-HDMI board, and connect
the X-IMX-MIPI-HDMI convert board with the monitor.
If use monitor to display through LVDS, connect J1 and X-IMX-LVDS-HDMI board, and connect
the X-IMX-LVDS-HDMI convert board with the monitor.

Prepare the Demo
================
1.  Connect 12V power supply and J-Link Debug Probe to the board, switch SW3 to power on the board.
2.  Connect a micro USB cable between the host PC and the J11 USB port on the cpu board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Launch the debugger in your IDE to begin running the example.

If use monitor to display through MIPI DSI, change
    #define APP_DISPLAY_EXTERNAL_CONVERTOR 0
    to
    #define APP_DISPLAY_EXTERNAL_CONVERTOR 1
    in dpu_example.h.

If use monitor to display through LVDS, change
    #define APP_DISPLAY_EXTERNAL_CONVERTOR 0
    to
    #define APP_DISPLAY_EXTERNAL_CONVERTOR 1
    and change
    #define DPU_EXAMPLE_DI DPU_DI_MIPI
    to
    #define DPU_EXAMPLE_DI DPU_DI_LVDS
    in dpu_example.h.

Running the demo
================
When the example runs successfully, you can see the rectangles and diamonds
moving in the screen.

NOTE: Before load and run the project, press SW3 until D3 is off, then press
SW3 until D3 is on. This perform a clean power on.
Customization options
=====================

