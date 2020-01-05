Overview
========
The 3-phase permanent magnet synchronous (PMSM) motor control demo application 
is based on Kinetis® MCUs and intended to provide the example for 3-phase sensorless 
PMSM motor control solutions. The reference design uses a closed-loop field-oriented 
vector speed (FOC) control mechanism.

Toolchain supported
===================
- IAR embedded Workbench 8.32.3
- Keil MDK 5.27
- MCUXpresso IDE 11.0

Hardware requirements
=====================
- Mini/micro USB cable
- HVP-KE18F MCU module 
- High-Voltage Platform (HVP-MC3PH)
- MIGE 60CST-MO1330 motor
- Personal Computer

Boards settings
===============
No special settings are required.

Prepare the Demo
================
1. Make sure the HVP-MC3PH main board is not plugged to the power source.
2. Insert the HVP-KE18F daughter board to the HVP-MC3PH main board (there is only one possible option).
3. Connect the BLDC motor 3-phase wires to the screw terminals on the board.
4. Plug the USB cable from the USB host to the OpenSDA USB connector.
5. Plug the 230 V power supply to the power connector, and switch it on.
6. Download the program to the target board.
7. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
Demo mode is turned OFF by default - Demo mode can be turned ON via FreeMaster.
Motor spinning can be controlled also by FreeMASTER file.

More information
================
For more information visit the page: http://www.nxp.com/motorcontrol_pmsm.
The documentation is available on the web page: http://www.nxp.com/motorcontrol_pmsm.