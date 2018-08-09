Overview
========
This application represents the "802.15.4 Server" of the Over-the-Air Programming Demo, and it demonstrates the following:
- channel selection algorithm
- the creation of a Personal Area Network (PAN)
- the Over-the-Air image transfer to a Client application.
This demo requires the "TestTool" PC application to be used with the OTAP Server through a serial interface.
For more information please refer to the "IEEE 802.15.4 MAC Demo Applications User's Guide.pdf" document.

Toolchain supported
===================
- IAR Embedded Workbench 7.80.4
- MCUXpresso IDE 10.0

Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-KW41Z board
- Personal Computer with TestTool 12 installed

Board settings
==============
No special board setting.

Prepare the Demo
================
1.  Connect a mini/micro USB cable between the PC host and the OpenSDA USB port on the board.
2.  Download the program to the target board.
3.  Press the reset button, then any other switch button on your board to begin running the demo

Running the demo
================
Open TestTool 12 and select "OTA Updates"->"OTAP 802.15.4 MAC".
Browse for the image to be transferred Over-the-Air.
Connect to the Server device and press the "Start OTAP Image Load To Server" button.