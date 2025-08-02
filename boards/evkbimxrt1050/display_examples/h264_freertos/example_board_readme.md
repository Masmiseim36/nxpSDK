Hardware requirements
=====================
- Micro USB cable
- EVKB-IMXRT1050 board
- JLink Plus
- 5V power supply
- Personal Computer
- RK043FN02H-CT or RK043FN66HS-CT6 LCD board
  (RK043FN02H-CT and RK043FN66HS-CT6 are compatible)
- SD card

Board settings
============
1. Connect the RK043FN02H-CT or RK043FN66HS-CT6 to board.
2. Prepare the H264 files and save in SD card.
3. Insert the SDCARD into card slot.

Prepare the Demo
================
The demo use RK043FN66HS-CT6 panel by default, to use other panel,
set the coresponding panel DEMO_PANEL_xxx(without '_SUPPORT') to y in prj.conf under board example folder before project generation.
The supported panels can be found in examples\_boards\<board>\project_segments\display_support\Kconfig.prjseg.
Or change the macro DEMO_PANEL in mcux_config.h of the project,
which number represents which panel can be found in display_support.h
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Launch the debugger in your IDE to begin running the demo.

Running the demo
===============
If this example runs correctly, pictures are shown in the lcd panel.
