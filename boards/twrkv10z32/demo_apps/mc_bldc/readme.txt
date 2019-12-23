Hardware requirements
=====================
- Mini/micro USB cable
- TWR-KV10Z MCU module 
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

TWR-KV10Z jumper settings:
Jumper  Setting  Jumper  Setting  Jumper  Setting
J1      2-3      J10     2-3      J21     3-4
J2      1-2      J11     open     J22     3-4
J3      2-3      J12     open     J25     open
J4      1-2      J13     open     J26     1-2
J5      1-2      J14     open     J27     1-2
J7      1-2      J18     2-3      J28     1-2
J8      2-3      J19     2-3      J29     1-2
J9      1-2      J20     2-3       —       —

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
    •  Connect the TWR MCU module to the host PC via a USB cable connected to J23 of the 
       TWR-KV10Z32 MCU module or J21 of the TWR-KV31F120 MCU module and any USB 
       port on the host PC. 

Running the demo
================
Press button SW2 on TWR-KV10Z board to switch on or off demonstration mode - the motor will rotate. 

More information
================
For more information visit the page: http://www.nxp.com/motorcontrol_bldc and find the doccument BLDCAPPACUG - 3-Phase Brushless DC (BLDC) Motor control User Guides