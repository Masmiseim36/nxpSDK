Overview
========


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
- FRDMK64F board
- FRDM-FXS-MULT2-B board
- Personal Computer

Software requirements
Sensor Fusion Toolbox for Windows
http://www.nxp.com/products/sensors/nxp-sensor-fusion:XTRSICSNSTLBOX


Board settings
==============
On FRDM-FXS-MULT2-B sensor shield board:
---------------------------------------
For I2C Mode: The examples use I2C1 and Pins 2-3 of Jumpers J27 and J28 on FRDM-FXS-MULT2-B should be connected.

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Build the project and download the program to the target board.
3.  Launch the debugger in your IDE to begin running the baremetal Sensor Fusion demo.

Running the demo
================
1.  Open Sensor Fusion Toolbox for Windows and click on auto detect to detect the COM port.
2.  Choose a sensor fusion algorithm and rotate the board to visualization of fusion results.
3.  For more details, refer to the "Help" tab of the Sensor Fusion Toolbox for Windows.
Customization options
=====================

