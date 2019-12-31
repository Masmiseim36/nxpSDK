Overview
========
The E-Compass demo application demonstrates the use of the FXOS8700 sensor. The tilt-compensated algorithm calculates
all three angles (pitch, roll, and yaw or compass heading).

Toolchain supported
===================
- IAR embedded Workbench 8.11.1
- Keil MDK 5.23
- GCC ARM Embedded 2016-5.4-q3
- Kinetis Development Studio IDE 3.2.0
- MCUXpresso10.3.1

Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-KW36 board
- Personal Computer

Board settings
==============

Prepare the Demo
================

1.  Connect a USB cable between the PC host and the OpenSDA USB port on the board.
2.  Open a serial terminal with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running
    the demo.

For more detailed instructions, see a Kinetis SDK User's Guide for your board.

Running the demo
================
When the example runs successfully, you can see the similar information from the terminal as below.

To calibrate Magnetometer, roll the board on all orientations to get max and min values

Press any key to start calibrating...

Calibrating magnetometer...
Calibrate magnetometer successfully!
Magnetometer offset Mx: 227 - My: 90 - Mz: 424 

Compass Angle: 137.9
Compass Angle: 138.5
Compass Angle: 138.8
Compass Angle: 138.6
...
Customization options
=====================

