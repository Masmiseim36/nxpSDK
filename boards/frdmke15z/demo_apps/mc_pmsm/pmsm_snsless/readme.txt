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
- FRDM-KE15Z board
- FRDM-MC-LVPMSM low-voltage evaluation board 
- Linix 45ZWN24-40 motor (for sensorless three wires cable only) 
- Personal Computer

Boards settings
===============
Before you power-up FRDM-KE15Z, ensure that the jumpers 
on your FRDM-KE15Z are configured as follows: 

FRDM-KE15Z jumper settings: 
Jumper  Setting  Function 
J15     2-3      VDD - 3.3V. 

Prepare the Demo
===============
1.  Connect the FRDM-MC-LVPMSM shield on top of the FRDM-KE15Z board. 
2.  Connect the BLDC motor 3-phase wires into the screw terminals on the board. 
3.  Plug in the USB cable from the USB host to the OpenSDA micro USB connector. 
4.  Plug in a 24 V DC power supply to the DC power jack. 

Running the demo
================
Press button SW2 on FRDM-KE15Z board to switch on or off demonstration mode - the motor will rotate. 
Motor spinning can be controlled also by FreeMASTER file.

More information
================
For more information visit the page: http://www.nxp.com/motorcontrol_pmsm.
The documentation is available on the web page: http://www.nxp.com/motorcontrol_pmsm.