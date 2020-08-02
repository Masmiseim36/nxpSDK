Overview
========

This document explains the freertos_queue example. This example introduce simple logging mechanism
based on message passing.

Example could be devided in two parts. First part is logger. It contain three tasks:
log_add().....Add new message into the log. Call xQueueSend function to pass new message into
              message queue.
log_init()....Initialize logger (create logging task and message queue log_queue).
log_task()....Task responsible for printing of log output.

Second part is application of this simple logging mechanism. Each of two tasks write_task_1 and
write_task_2 print 5 messages into log.



Toolchain supported
===================
- Keil MDK 5.24a
- IAR embedded Workbench 8.22.2
- GCC ARM Embedded 7-2017-q4-major
- MCUXpresso10.2.0

Hardware requirements
=====================
- J-Link ARM
- Mini USB cable
- MAPS-KS22F256 board & MAPS-DOCK board
- Personal Computer

Board settings
==============
The hello_world demo is configured to use UART1 with PTE0 and PTE1 pins. To make JLink serial
device work, the jumpers on MAPS-DOCK board should be set as following:
- JP7 on MAPS-Dock: UART0 related jumpers connected
Prepare the Demo
================

Running the demo
================
After the board is flashed the Tera Term will show debug console output.

Example output:
Log 0: Task1 Message 0
Log 1: Task2 Message 0
Log 2: Task1 Message 1
Log 3: Task2 Message 1
Log 4: Task1 Message 2
Log 5: Task2 Message 2
Log 6: Task1 Message 3
Log 7: Task2 Message 3
Log 8: Task1 Message 4
Log 9: Task2 Message 4
Customization options
=====================

