Overview
========
The FXLC95000CL Intelligent, Motion-Sensing Platform is a device with the integration of a 3-axis MEMS accelerometer and a 32-bit ColdFire MCU.
This environment allows users with widely varying and growing functional requirements the ability to add user-specific functionality to the system.

A software framework hosted on the FXLC95000CL provides a common, easy to use interface which empowers users to quickly develop hardware-independent, portable, real-time application code.
This framework is named the Xtrinsic Intelligent Sensing Framework (ISF).

The FXLC95000L Accelerometer SPI example application demonstrates the use of the FXLC95000L as an Accelerometer in SPI Mode with ISF1P195K GetAccelDataWithCI is running on 32-bit ColdFire MCU.

The example demonstrates all commands reguired to put the sensor into Active Mode and read out Accelerometer samples.
The Accelerometer reads samples as per the configured ODR and updates the output mailbox registers.
The application reads out samples when its ODR Timer expires.

Toolchain supported
===================
- IAR embedded Workbench 7.80.4
- Keil MDK 5.21a
- GCC ARM Embedded 2016-5.4-q3
- Kinetis Development Studio IDE 3.2.0
- MCUXpresso0.8

Hardware requirements
=====================
- Mini/micro USB cable
- FRDMK22F board
- FRDM-STBC-SA9500 board
- Personal Computer

Board settings
==============
On FRDM-STBC-SA9500 sensor shield board:
--------------------------------------
For SPI Mode: The examples use SPI and Pins 1-2 of Jumpers J10 on FRDM-STBC-SA9500 should be connected.

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the demo runs successfully, you can see the Samples printed to the terminal.

Note: you must rotate the board to get the samples to change.
Customization options
=====================

