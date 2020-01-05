Overview
========
This document explains the freertos_tickless example. It shows how the CPU enters the sleep mode and then
it is woken up either by expired time delay using low power timer module or by external interrupt caused by a
user defined button.


Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- Keil MDK  5.29
- GCC ARM Embedded  8.3.1
- MCUXpresso  11.1.0

Hardware requirements
=====================
- J-Link ARM
- Mini USB cable
- TWR-K80F150M board
- Personal Computer

Board settings
==============
This example project does not call for any special hardware configurations.
Although not required, the recommendation is to leave the development board's jumper settings
and configurations in default state when running this example.


Prepare the Demo
================
1. Connect a mini USB cable between the PC host and the OpenSDA USB port (J2) on twrk64f120m board.
2. Open a serial terminal on PC for OpenSDA serial device with these settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running
   the demo. For detailed instructions, see Getting Started with Software Development Kit for
   K64F (document SDKK64FGSUG).

Running the demo
================
When the demo runs successfully, output can be seen on the OpenSDA terminal.
Customization options
=====================

