Overview
========

The src_reset_source example shows how to check the reset source and boot option with SRC driver.

When run this example firstly on the board after power up, the POR reset or IPP reset flag would be asserted. But when running it again, the flag would be cleared, since it is no longer a reset.

Usually, the reset flags are used and cleared by ROM code or bootloader to check which reset occurred. With the boot mode words, the initial boot code would go to the right way to execute the start code. For example, when the SoC is waked up from some low power mode, all necessary data are already staying in the RAM, the CPU would not have to reload the program to RAM and execute the application directly.


Toolchain supported
===================
- IAR embedded Workbench 7.80.4
- GCC ARM Embedded 2016-5.4-q3

Hardware requirements
=====================
- Micro USB cable
- MCIMX6UL board (MCIMX6UL-CM and MCIMX6UL-BB)
- JLink Plus
- 5V power supply
- Personal Computer

Board settings
==============
No special is needed.

Prepare the Demo
================
1.  Connect 5V power supply and JLink Plus to the board, switch SW2001 to power on the board.
2.  MCIMX6UL-EVK -CM board boot settings: SW601: OFF-OFF-ON-OFF, SW602: ON-OFF
3.  Connect a USB cable between the host PC and the J1901 USB port on the target board.
4.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
5.  Download the program to the target board.
6.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the demo runs successfully, the log would be seen on the terminal like:

Example: SRC Reset Soruce.
SRC_GetResetStatusFlags(): 0x1.
SRC_GetBootModeWord1(): 0x852.
SRC_GetBootModeWord2(): 0x2000001.

Then Run the demo again, the log would be:
Example: SRC Reset Soruce.
SRC_GetResetStatusFlags(): 0x0.
SRC_GetBootModeWord1(): 0x852.
SRC_GetBootModeWord2(): 0x2000001.

Customization options
=====================

