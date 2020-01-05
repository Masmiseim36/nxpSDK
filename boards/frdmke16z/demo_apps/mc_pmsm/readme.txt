Overview
========
The 3-phase permanent magnet synchronous (PMSM) motor control demo application 
is based on Kinetis® MCUs and intended to provide the example for 3-phase sensorless 
PMSM motor control solutions. The reference design uses a closed-loop field-oriented 
vector speed (FOC) control mechanism.

Toolchain supported
===================
- IAR embedded Workbench 8.30.1
- Keil MDK 5.25
- MCUXpresso IDE 10.2.1

Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-KE16Z board
- FRDM-MC-LVPMSM low-voltage evaluation board 
- Linix 45ZWN24-40 motor (for sensorless three wires cable only) 
- Personal Computer

Boards settings
===============
Before you power-up FRDM-KE16Z, ensure that the jumpers 
on your FRDM-KE16Z are configured as follows: 

FRDM-KE16Z jumper settings: 
Jumper  Setting  Function 
J19     2-3      VDD - 3.3V. 

0-ohm resistor setting:
    MCU pin       Resistor    Add / Remove
-------------------------------------------
PTB4/RGB_GREEN      R24         remove
PTB4/MC_PWM_CT      R25         add
PTB5/RGB_RED        R28         remove
PTB5/MC_PWM_CB      R29         add
PTC0/GES_R0         R110        remove
PTC0/MC_PWM_AT      R111        add  
PTC1/GES_R1         R8          remove
PTC1/MC_PWM_AB      R13         add  
PTD1/RGB_BLUE       R6          remove
PTD1/MC_PWM_BB      R9          add
PTC0/GES_R0         R112        remove
PTB2/MC_VOLT_DCB    R113        add

Prepare the Demo
===============
1.  Connect the FRDM-MC-LVPMSM shield on top of the FRDM-KE16Z board. 
2.  Connect the BLDC motor 3-phase wires into the screw terminals on the board. 
3.  Plug in the USB cable from the USB host to the OpenSDA micro USB connector. 
4.  Plug in a 24 V DC power supply to the DC power jack. 

Running the demo
================
Press button SW2 on FRDM-KE16Z board to switch on or off demonstration mode - the motor will rotate. 

More information
================
For more information open the user guide in KSDK install folder\docs\MC\PMSMDEMOUG.pdf or visit the page: http://www.nxp.com/motorcontrol_pmsm and find the doccument PMSMCRSPUG - PMSM Control Reference Solution Package.
