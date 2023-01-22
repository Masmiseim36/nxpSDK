
Toolchain supported
===================
- IAR embedded Workbench  9.32.1
- GCC ARM Embedded  10.3.1

Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT1170-EVK board
- Personal Computer
- RK055MHD091 panel or RK055AHD091 panel or RK055IQH091 panel

Board settings
==============
Connect the panel to J48
Connect 5V power to J43, set J38 to 1-2

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Build the project, the project uses the RK055MHD091 panel.
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the example runs, you can see a tiger with high render quality on white buffer.

Running the ARMGCC demo under GDB

Assuming your $PATH variable points to arm-none-eabi-gdb:

    $ arm-none-eabi-gdb    sdram_release/openvg_tiger_freertos.elf
    target remote localhost:2331
    monitor reset
    load
    monitor semihosting enable
    monitor go
    continue
