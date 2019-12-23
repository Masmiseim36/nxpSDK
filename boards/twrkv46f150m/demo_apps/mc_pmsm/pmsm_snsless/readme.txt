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
- TWR-KV46F MCU module 
- Three-phase low-voltage power module (TWR-MC-LV3PH) 
- Linix 45ZWN24-40 motor (for sensorless three wires cable only) 
- Personal Computer

Boards settings
===============
Before you insert the TWR-MC-LV3PH module into the Tower System, ensure that the jumpers
on your TWR-MC-LV3PH and TWR-KVxxXxx MCU modules are configured as follows:

TWR-MC-LV3PH jumper settings:
Jumper  Setting
J2      1-2
J3      1-2
J10     1-2
J11     1-2
J12     1-2
J13     1-2
J14     1-2

TWR-KV46F jumper settings 
Jumper  Setting  Jumper  Setting  Jumper  Setting 
J1      open     J16     open     J505    3-4 
J2      open     J19     open     J506    3-4 
J4      2-3      J20     1-2      J512    1-2 
J5      1-2      J21     open     J514    2-3
J13     1-2,3-4  J23     open     J517    2-3 
J15     1-2      J502    1-2      J519    3-4

- see board jumper setting pictures in documentation on the web page http://www.nxp.com/motorcontrol_pmsm.

Prepare the Demo
===============
1.  Insert the TWR-KVxxXxx MCU module and the TWR-MC-LV3PH module into the 
    TWR-ELEV card slots. Ensure that the primary sides of the modules (marked by white stripes) 
    are inserted into the primary elevator card (marked by white connectors). 
2.  After assembling the Tower System, connect the required cables as follows: 
    •  Connect the power input cable (3-wire connector) of the Linix motor to its corresponding 
       connector (J5) on the TWR-MC-LV3PH motor control driver board. 
    •  Plug in the power supply cable that is attached to the TWR-MC-LV3PH system kit to the 
       motor control peripheral board TWR-MC-LV3PH. 
    •  Connect the TWR MCU module to the host PC via a USB cable.

Running the demo
================
Press button SW2 on TWR-KV46F board to switch on or off demonstration mode - the motor will rotate. 
Motor spinning can be controlled also by FreeMASTER file.

More information
================
For more information visit the page: http://www.nxp.com/motorcontrol_pmsm.
The documentation is available on the web page: http://www.nxp.com/motorcontrol_pmsm.