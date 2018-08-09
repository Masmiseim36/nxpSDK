Overview
========
This application represents the "Thread end device ota client" of the Kinetis Thread Stack Demo Applications.
The main characteristic of an End Device is that it always communicates to the rest of the 
network by having data relayed to and from an Active Router which becomes a "parent" for the
End Device. 
The End Device at its turn acts as the "child" of the Active Router and expects the Active
Router to act on its behalf for forwarding data transfers.
The OTA Client receives a new image over the air ant write to the external storage.
The bootloader update the internal flash at the next reboot.
For more information please refer to the "Kinetis Thread Stack Demo Applications User's Guide.pdf" document
and "Kinetis Thread Stack Over-the-Air (OTA) Firmware Update User’s Guide.pdf".

Toolchain supported
===================
- IAR embedded Workbench 7.80.4
- MCUXpresso IDE 10.0

Hardware requirements
=====================
- USB-KW41Z board
- Personal Computer

Board settings
==============
No special board setting.

Prepare the Demo
================
1.  Connect the board to the PC host.
2.  Download the Bootloader program to the target board.
3.  Download the program to the target board.
4.  Press the reset button, then any other switch button on your board to begin running the demo.

Running the demo
================
Follow the instructions from the "Kinetis Thread Stack Demo Applications User's Guide.pdf" and 
"Kinetis Thread Stack Over-the-Air (OTA) Firmware Update User’s Guide.pdf".