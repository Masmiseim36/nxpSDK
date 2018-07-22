Overview
========
The MPL3115 Altitide example application demonstrates the use of the MPL3115 sensor to read Altitide instead of Pressure.
The example demonstrates configuration of all registers reguired to put the sensor into Altitude mode and read Altitude+Temperature samples.

The sensor reads samples as per the configured Auto acquisition time step and sets a Flag.
The application consistently checks the Flag and when set reads out the sample.

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
- FRDM-K22F board
- FRDM-STBC-AGMP03 board
- Personal Computer

Board settings
==============
On FRDM-STBC-AGMP03 sensor shield board:
---------------------------------------
Since the examples use I2C1 Pins 2-3 of Jumpers J7 and J8 on FRDM-STBC-AGMP03 should be connected.
SW2 should be switched to "ACCEL NORMAL" mode and SW3 should be switched to "ACCEL-GYRO-I2C" mode.

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
Customization options
=====================

