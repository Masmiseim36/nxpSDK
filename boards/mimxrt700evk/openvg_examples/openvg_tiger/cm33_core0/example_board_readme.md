Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT700-EVK board
- Personal Computer
- RK055MHD091 panel or RK055AHD091 panel or RK055IQH091 panel

Board settings
============
Connect the panel to J52.
Switch the J45 jumper to the 1-2 configuration to enable XSPI1 PSRAM memory access.
Switch the SW10 boot mode switch to the 01 XSPI0 flash configuration.

Prepare the Demo
===============
1.  Connect a micro USB cable between the PC host and the CMSIS DAP USB port (J54) on the board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Build the project, the project uses the RK055MHD091 panel.
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
===============
When the example runs, you can see a tiger with high render quality on white buffer.

Running the ARMGCC demo under GDB
=================================

Assuming your $PATH variable points to arm-none-eabi-gdb:

    $ arm-none-eabi-gdb release/openvg_tiger_freertos_cm33_core0.elf
    target remote localhost:2331
    monitor reset
    load
    monitor go
    continue
