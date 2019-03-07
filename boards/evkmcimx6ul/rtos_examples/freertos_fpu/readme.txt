Overview
========

The freertos_fpu example shows an application using RTOS tasks with FPU count to test FPU context
save/restore support in FreeRTOS.

Example could be devided in two parts. First part is FPU count. It contain two tasks:
sin_task()....Do sin() count and judge the result. It will print the error number if result error.
cos_task()....Do cos() count and judge the result. It will print the error number if result error.
This two task have equal priority and the RTOS scheduler will switch between these two tasks 
on every RTOS tick interrupt.

Second part is a periodic interrupt handle of gtp timer. This interrupt will do a single FPU count and
will make sin_task/cos_task appear error if without FPU context save/restore.



Toolchain supported
===================
- IAR embedded Workbench 7.80.4
- GCC ARM Embedded 2016-5.4-q3

Hardware requirements
=====================
- Micro USB cable
- MCIMX6UL-EVK  board
- JLink Plus
- 5V power supply
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
1.  Connect 5V power supply and JLink Plus to the board, switch SW2001 to power on the board.
2.  Connect a USB cable between the host PC and the J1901 USB port on the target board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
After the board is flashed the Tera Term will show debug console output.

Example output:
sin_task FPU test succeed!
cos_task FPU test succeed!
sin_task FPU test succeed!
cos_task FPU test succeed!
sin_task FPU test succeed!
cos_task FPU test succeed!
sin_task FPU test succeed!
cos_task FPU test succeed!
sin_task FPU test succeed!
cos_task FPU test succeed!
Customization options
=====================

