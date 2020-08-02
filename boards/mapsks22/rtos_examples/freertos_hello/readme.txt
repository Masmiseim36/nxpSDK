Overview
========
The Hello World project is a simple demonstration program that uses the SDK UART drivere in
combination with FreeRTOS. The purpose of this demo is to show how to use the debug console and to
provide a simple project for debugging and further development.

The example application creates one task called hello_task. This task print "Hello world." message
via debug console utility and suspend itself.




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
After the board is flashed the Tera Term will print "Hello world" message on terminal.

Example output:
Hello world.
Customization options
=====================

