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
- FRDM-KV11Z board
- FRDM-MC-LVPMSM low-voltage evaluation board 
- Linix 45ZWN24-40 motor (for sensorless three wires cable only) 
- Personal Computer

Boards settings
===============
The MCU board does not require any hardware configuration or jumper settings. 
It contains no jumpers. The low-voltage evaluation board does not require any hardware configuration or jumper settings as well. 

Prepare the Demo
===============
1.  Connect the FRDM-MC-LVPMSM shield on top of the FRDM-KV11Z board (there is only one 
    possible option). 
2.  Connect the Linix motor three-phase wires into the screw terminals on the board. 
3.  Plug the USB cable from the USB host to the OpenSDA micro USB connector. 
4.  Plug the 24 V DC power supply to the DC Power connector.  

Running the demo
================
Press button SW2 on FRDM-KV11Z board to switch on or off demonstration mode - the motor will rotate. 
Motor spinning can be controlled also by FreeMASTER file.

More information
================
For more information visit the page: http://www.nxp.com/motorcontrol_pmsm.
The user guide is in SDK install folder\docs\MC\PMSMDEMOUG.pdf.