Hardware requirements
=====================
- Micro USB cable
- JTAG/SWD debugger
- MIMXRT685-AUD-EVK board
- Personal Computer

Board settings
==============

Prepare the Demo
===============
1.  Connect a micro USB cable between the PC host and the debug USB port (J5) on the board
2.  Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program for CM33 core to the target board.
4.  Launch the debugger in your IDE to begin running the demo.
5.  If building debug configuration, download the program for DSP core to the target board.
6.  If building debug configuration, launch the Xtensa IDE or xt-gdb debugger to
begin running the demo.

NOTE: DSP image can only be debugged using J-Link debugger.  See
'Getting Started with Xplorer for MIMXRT685-AUD-EVK.pdf' for more information.

Running the demo CM33
===============
When the demo runs successfully, the terminal will display the following:

    Hello World running on core 'Cortex-M33'

Running the demo DSP
===============
When the demo runs successfully, the terminal will display the following:

    Hello World running on DSP core 'nxp_rt600_RI2020_5_newlib'

