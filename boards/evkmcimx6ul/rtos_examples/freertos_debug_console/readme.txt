Overview
========
The Debug console example for FreeRTOS demonstrates the possibility to use the Debug console in the RTOS.

Toolchain supported
===================
- IAR embedded Workbench 7.80.4
- GCC ARM Embedded 2016-5.4-q3

Hardware requirements
=====================
- Micro USB cable
- MCIMX6UL board
- JLink Plus
- 5V power supply
- Personal Computer

Board settings
==============
No special settings are required.

Debug console configuration
Debug console should be configured through fsl_debug_console_conf.h.
New Debug console support multi task log printf and scanf also as a buffer is added between the debug console and IO.
The debug console allow user select device, buffer size, support polling and interrupt transfer.
We are using freertos to demonstrate the debug console's functionality, if debug console is porting to other RTOS, user should implement the mutex, semaphore according to RTOS provide for debug console(fsl_io.c, fsl_log.c) ,otherwise the log will be messed on the terminal.

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
Debug console Demo-print task2.
c
Task2 priority 4, stack size is 100, current tick count 10635620

Debug console Demo-print task1.

Task1 priority 4, stack size is 100, current tick count 10635620

Debug console Demo-print task3.

Task3 priority 3, stack size is 100, current tick count 10635620

Debug console Demo-print task1.

Task1 priority 4, stack size is 100, current tick count 10635720

Debug console Demo-print task1.

Task1 priority 4, stack size is 100, current tick count 10635820

Customization options
=====================

