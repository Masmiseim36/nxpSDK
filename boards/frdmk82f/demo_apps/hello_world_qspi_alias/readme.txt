Overview
========
The Hello World QSPI demo application provides a sanity check for new SDK build environments and board bring up. It
prints the "Hello World" string to the terminal using the SDK UART drivers. The purpose of this demo is to show how to
compile SDK applications to run and boot from QSPI flash or a QSPI alias region.

This example can generate two different demos using a different linker file and a startup file:
Program boot from QSPI region.

Program boot from QSPI alias region (if the chip supports QSPI alias region).


Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- Keil MDK  5.29
- GCC ARM Embedded  8.3.1
- MCUXpresso  11.1.0

Hardware requirements
=====================
- Mini/Micro USB cable
- FRDM-K82F board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
On the FRDMK-82 board, the user can use the USB port to download the demo program through blhost. At step 1, press the
SW2 button and connect to the USB cable to the target hardware.

Running the demo
================
When the program is downloaded via blhost, just re-connect the OpenSDA port, and press reset button. After the target
processor resets, the demo application will print hello world to the terminal and echo the characters received from the
user to the screen.

hello world
Customization options
=====================

