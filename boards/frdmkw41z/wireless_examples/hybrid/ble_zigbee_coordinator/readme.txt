Overview
========
This application represents the "Hybrid Coordinator" Zigbee 3.0 demo application.
The Coordinator is responsible for initially forming the network and then, via the Trust Centre functionality, managing which other devices are allowed to join the network and distributing security materials to those that are allowed to join.
The application for BLE implements a custom GATT based Wireless UART Profile that emulates UART over BLE for the ZigBee Shell.
For more information please refer to the "AN12062-MKW41Z-AN-ZigBee-3-0-Base-Device-BLE-Dual-Mode.pdf" document.

Toolchain supported
===================
- IAR Embedded Workbench v8.32.1
- MCUXpresso IDE v10.2.1

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
Follow the instructions from the "AN12062-MKW41Z-AN-ZigBee-3-0-Base-Device-BLE-Dual-Mode.pdf" document.