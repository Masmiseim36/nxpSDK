Overview
========
The 3-phase brushless DC (BLDC) motor control demo application is based 
on Kinetis® MCUs and intended to provide the example for 3-phase sensorless 
BLDC motor control solution. The demo design uses a six-step communication process,
including closed-loop speed control and dynamic motor current limitation. 

Toolchain supported
===================
- IAR embedded Workbench 8.32.3
- Keil MDK 5.27
- MCUXpresso IDE 11.0.0

Hardware requirements
=====================
- Mini/micro USB cable
- TWR-KV11Z MCU module 
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
J10     2-3
J11     2-3
J12     2-3
J13     2-3
J14     2-3

TWR-KV11Z jumper settings:
Jumper  Setting             Jumper   Setting             Jumper  Setting
J1      1-2                 J9       open                J505    2-3
J2      2-3                 J10      1-2                 J506    2-3
J4      2-3                 J11      2-3                 J512    1-2
J5      5-6,7-8,9-10        J12      1-2                 J517, J518  J518-J517(2)
J6      open                J13      2-3                 J519    1-2
J7      open                J14      1-2                 J523    1-2
J8      open                J17      2-3                 J524    open
—        —                   —        —                  J526    1-2

- see board jumper setting pictures in \docs\MC\BLDCDEMOUG.pdf

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
Press button SW2 on TWR-KV11Z board to switch on or off demonstration mode - the motor will rotate. 

More information
================
For more information visit the page: http://www.nxp.com/motorcontrol_bldc and find the doccument BLDCAPPACUG - 3-Phase Brushless DC (BLDC) Motor control User Guides