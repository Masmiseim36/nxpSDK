Overview
========
The E-Compass demo application demonstrates the use of the FXOS8700 sensor. The tilt-compensated algorithm calculates
all three angles (pitch, roll, and yaw or compass heading).

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
- FRDM-KW41Z board
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

Initialization of hwtimer1   :
== Accelerometer Test ==

Freescale Magnetic Calibration and eCompass Software v3.00
0: Display calibration parameters
1: Run eCompass with 10, 7 or 4 element calibration
Calibration hard iron (uT) Vx=        0 Vy=        0 Vz=        0

Calibration inverse soft iron matrix invW (normalized)
Row 0     1.00000           0           0
Row 1           0     1.00000           0
Row 2           0           0     1.00000
Angles: Phi=Roll, Theta=Pitch, Psi=Yaw, Rho=Compass, Delta=inclination
Iteration:      0
f6DOF: Gpx    -23  -1.01 Gpy    -33  -1.02 Gpz   2012   0.98
f6DOF: Bpx      0      0 Bpy      0      0 Bpz   -256 -26.60
f6DOFECOM: Phi    0.65 The  179.06 Psi  325.12 Rho   34.88 Delta  -89.85
f6DOFLP  : Phi    0.65 The  179.06 Psi  325.12 Rho   34.88 Delta  -89.85
1 entries in magnetometer buffer is too few for calibration
Customization options
=====================

