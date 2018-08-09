Overview
========
This application represents the "Thread router eligible device" of the Kinetis Thread Stack Demo Applications.
A Router Eligible Device is a node which initially joins the network as an End Device, but can adaptively 
become a mesh Router. Such a device may also have capabilities to initialize, create, and bootstrap 
a new Thread Network for the user or a management entity.
The OTA Client receives a new image over the air and write to the external storage.
The bootloader update the internal flash at the next reboot.
For more information please refer to the "Kinetis Thread Stack Demo Applications User's Guide.pdf" document
and "Kinetis Thread Stack Over-the-Air (OTA) Firmware Update User’s Guide.pdf".

Toolchain supported
===================
- IAR embedded Workbench 7.80.4
- MCUXpresso IDE 10.0

Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-KW41Z board
- Personal Computer

Board settings
==============
No special board setting.

Prepare the Demo
================
1.  Connect a mini/micro USB cable between the PC host and the OpenSDA USB port on the board.
2.  Download the Bootloader program to the target board.
3.  Download the program to the target board.
4.  Press the reset button, then any other switch button on your board to begin running the demo.

Running the demo
================
Follow the instructions from the "Kinetis Thread Stack Demo Applications User's Guide.pdf" and
"Kinetis Thread Stack Over-the-Air (OTA) Firmware Update User’s Guide.pdf".