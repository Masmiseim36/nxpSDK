Hardware requirements
===================
- Mini/micro USB cable
- FRDMK22F board
- FRDM-STBC-AGMP03 board
- Personal Computer

Software requirements
===================
Sensor Fusion Toolbox for Windows
http://www.nxp.com/products/sensors/nxp-sensor-fusion:XTRSICSNSTLBOX

Board settings
==============
On FRDM-STBC-AGMP03 sensor shield board:
--------------------------------------
Since the examples use I2C1 Pins 2-3 of Jumpers J7 and J8 on FRDM-STBC-AGMP03 should be connected.
SW2 should be switched to "ACCEL NORMAL" mode and SW3 should be switched to "ACCEL-GYRO-I2C" mode.

Demo Overview
============
The freeRTOS Sensor Fusion example for FRDM-K22F with FRDM-STBC-AGMP03 sensor shield board
demonstrates advanced functions for computation of device orientation, linear acceleration,
gyro offset and magnetic interference based on the outputs of NXP inertial and magnetic sensors.
The sensor fusion algorithms combine the x, y and z outputs from each of an accelerometer,
magnetometer and gyroscope sensor.


Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Build the project and download the program to the target board.
3.  Launch the debugger in your IDE to begin running the freeRTOS Sensor Fusion demo.

Running the demo
===============
1.  Open Sensor Fusion Toolbox for Windows and click on auto detect to detect the COM port.
2.  Choose a sensor fusion algorithm and rotate the board to visualization of fusion results.
3.  For more details, refer to the "Help" tab of the Sensor Fusion Toolbox for Windows.
