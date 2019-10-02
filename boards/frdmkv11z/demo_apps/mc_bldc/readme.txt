Overview
========
The 3-phase brushless DC (BLDC) motor control demo application is based 
on Kinetis® MCUs and intended to provide the example for 3-phase sensorless 
BLDC motor control solutions. The demo design uses a six-step communication process,
including closed-loop speed control and dynamic motor current limitation. 

Toolchain supported
===================
- IAR embedded Workbench 8.32.3
- Keil MDK 5.27
- MCUXpresso IDE 11.0.0

Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-KV11Z board
- FRDM-MC-LVBLDC low-voltage evaluation board 
- Linix 45ZWN24-40 motor (for sensorless three wires cable only) 
- Personal Computer

Boards settings
===============
The MCU board does not require any hardware configuration or jumper settings. 
It contains no jumpers. The low-voltage evaluation board does not require any hardware configuration or jumper settings as well. 

Prepare the Demo
===============
1.  Connect the FRDM-MC-LVBLDC shield on top of the FRDM-KV11Z board. 
2.  Connect the BLDC motor 3-phase wires into the screw terminals on the board. 
3.  Plug in the USB cable from the USB host to the OpenSDA micro USB connector. 
4.  Plug in a 12 V DC power supply to the DC power jack. 

Running the demo
================
Press button SW2 on FRDM-KV11Z board to switch on or off demonstration mode - the motor will rotate. 

More information
================
For more information open the user guide in KSDK install folder\docs\MC\BLDCDEMOUG.pdf or visit the page: http://www.nxp.com/motorcontrol_bldc and find the doccument BLDCAPPACUG - 3-Phase Brushless DC (BLDC) Motor control User Guides.
