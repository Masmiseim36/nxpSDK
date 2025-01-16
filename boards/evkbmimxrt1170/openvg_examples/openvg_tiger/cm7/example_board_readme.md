Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT1170-EVKB board
- Personal Computer
- RK055MHD091 panel or RK055AHD091 panel or RK055IQH091 panel

Board settings
============
Connect the panel to J48
Connect 5V power to J43, set J38 to 1-2

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Build the project, the project uses RK055MHD091 by default, to use other panels,
    change
    #define DEMO_PANEL 2
    to
    #define DEMO_PANEL 0
    or
    #define DEMO_PANEL 1
    or
    #define USE_MIPI_PANEL 3 in mcux_config.h
    Which number represent which panel can be found in display_support.h.
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
===============
When the example runs, you can see a tiger with high render quality on white buffer.

Running the ARMGCC demo under GDB
=================================

Assuming your $PATH variable points to arm-none-eabi-gdb:

    $ arm-none-eabi-gdb sdram_release/openvg_tiger_freertos_cm7.elf
    target remote localhost:2331
    monitor reset
    load
    monitor semihosting enable
    monitor go
    continue
