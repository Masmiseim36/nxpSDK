Hardware requirements
=====================
- Mini/micro USB cable
- TWR-KV58F MCU module 
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

TWR-KV58F jumper settings 
Jumper  Setting  Jumper  Setting  Jumper  Setting 
J1      1-2      J11     1-2      J23     2-3 
J2      open     J12     1-2      J24     2-3 
J3      1-2      J14     open     J25     2-3 
J4      1-2      J17     open     J26     2-3 
J7      1-2      J18     open     J28     1-2 
J8      1-2      J19     open     J29     open 
J9      open     J20     open     J30     1-2, 3-4, 5-6, 7-8 
J10     1-2      J21     1-2       —       —

- see board jumper setting pictures in \docs\MOTOR_CONTROL\BLDCDEMOUG.pdf

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
Press button SW2 on TWR-KV58F board to switch on or off demonstration mode - the motor will rotate. 

More information
================
For more information visit the page: http://www.nxp.com/motorcontrol_bldc and find the doccument BLDCAPPACUG - 3-Phase Brushless DC (BLDC) Motor control User Guides