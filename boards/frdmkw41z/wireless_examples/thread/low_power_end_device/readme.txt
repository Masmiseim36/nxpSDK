Overview
========
This application represents the "Thread low power end device" of the Kinetis Thread Stack Demo Applications.
The main characteristic of an End Device is that it always communicates to the rest of the 
network by having data relayed to and from an Active Router which becomes a "parent" for the
End Device. 
The End Device at its turn acts as the "child" of the Active Router and expects the Active
Router to act on its behalf for forwarding data transfers.
The Low-power End Device (also called Sleepy End Device (SED)) is an End Device which is meant
to remain in a low-power, dormant state for the majority of its lifetime. 
It is usually battery-powered with a limited battery capacity.
For more information please refer to the "Kinetis Thread Stack Demo Applications User's Guide.pdf" document.

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
2.  Download the program to the target board.
3.  Press the reset button, then any other switch button on your board to begin running the demo.

Running the demo
================
Follow the instructions from the "Kinetis Thread Stack Demo Applications User's Guide.pdf".