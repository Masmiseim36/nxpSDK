Hardware requirements
=====================
- Micro USB cable
- EVK-MIMXRT595 board
- Personal Computer

Board settings
============


Prepare the Demo
===============
The DSP images are built into CM33 image with default project configuration.
To build the CM33 image, the DSP images dsp_sram_release.bin, dsp_tcm_release.bin should be built firstly.
In addition, to build CM33 image with MCUXpresso IDE, the DSP images should be copied to
the MCUXpresso IDE workspace folder, like C:\Users\<user_name>\Documents\MCUXpressoIDE_11.0.1_2563\workspace\fusionf1.

1.  Connect a micro USB cable between the PC host and the CMSIS DAP USB port (J40) on the board
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Launch the debugger in your IDE to begin running the demo.
