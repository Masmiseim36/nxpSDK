Overview
========
The NXP Sensor Fusion Library for Kinetis MCUs (also referred to as Fusion Library or development kit)
provides advanced functions for computation of device orientation, linear acceleration, gyro offset
and magnetic interference based on the outputs of NXP inertial and magnetic sensors.

Toolchain supported
===================
- IAR embedded Workbench 7.70.1

Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-KL25Z board
- Any sensor shield board e.g. FRDM-STBC-AGM01 board
- Personal Computer

Software requirements
Sensor Fusion Toolbox for Windows
http://www.nxp.com/products/sensors/nxp-sensor-fusion:XTRSICSNSTLBOX

Board settings
==============
On FRDM-STBC-AGM01 sensor shield board:
--------------------------------------
For I2C Mode: The examples use I2C1 and Pins 2-3 of Jumpers J6 and J7 on FRDM-STBC-AGM01 should be connected.

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Apply following updates:
    - Update MKL25Z128xxx4_flash.icf file, change heap size in the linker config file from 0x400 to 0x1000
	- Change IAR project C compiler optimization to High-Balanced.
3.  Build the project and download the program to the target board.
4.  Launch the debugger in your IDE to begin running the baremetal Sensor Fusion demo.

Running the demo
================
1.  Open Sensor Fusion Toolbox for Windows and click on auto detect to detect the COM port.
2.  Choose a sensor fusion algorithm and rotate the board to visualization of fusion results.
3.  For more details, refer to the "Help" tab of the Sensor Fusion Toolbox for Windows.
Customization options
=====================

