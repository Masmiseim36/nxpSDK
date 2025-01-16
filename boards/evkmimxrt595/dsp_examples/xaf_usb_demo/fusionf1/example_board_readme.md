Hardware requirements
=====================
- 2x Micro USB cable
- JTAG/SWD debugger
- EVK-MIMXRT595 board
- Personal Computer
- Headphones with 3.5 mm stereo jack

Board settings
==============
Note: The I3C Pin configuration in pin_mux.c is verified for default 1.8V, for 3.3V,
need to manually configure slew rate to slow mode for I3C-SCL/SDA.

To enable the example audio using WM8904 codec, connect pins as follows:
- JP7-1        <-->        JP8-2

Prepare the Demo
================
1. Connect headphones to Audio HP / Line-Out connector (J4).
2. Connect the first micro USB cable between the PC host and the debug USB port (J40) on the
   board
3. Connect the second micro USB cable between the PC host and the USB port (J38) on the board.
4. Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
5. Download the program for CM33 core to the target board.
6. Launch the debugger in your IDE to begin running the demo.
7. If building debug configuration, start the xt-ocd daemon and download the program for
   DSP core to the target board.
8. If building debug configuration, launch the Xtensa IDE or xt-gdb debugger to begin
   running the demo.

**Notes**
- To be able to build the DSP project, please see the document
'Getting Started with Xplorer for EVK-MIMXRT595.pdf'.
- DSP image can only be debugged using J-Link debugger. See again
'Getting Started with Xplorer for EVK-MIMXRT595.pdf' for more information.

