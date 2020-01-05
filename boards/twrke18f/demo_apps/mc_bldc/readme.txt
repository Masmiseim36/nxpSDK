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
- TWR-KE18F MCU module 
- Three-phase low-voltage power module (TWR-MC-LV3PH) 
- Linix 45ZWN24-40 motor (for sensorless three wires cable only) 
- Personal Computer

Boards settings
===============
Before you insert the TWR-MC-LV3PH module into the Tower System, ensure that the jumpers 
on your TWR-MC-LV3PH and TWR-KExxXxx MCU modules are configured as follows: 

TWR-MC-LV3PH jumper settings: 
Jumper  Setting  Function 
J2      1-2      Selects the internal analog power supply. 
J3      1-2      Selects the internal analog power reference (GND). 
J10     2-3      Selects BEMF_SENSE_C. 
J11     2-3      Selects BEMF_SENSE_B. 
J12     2-3      Selects BEMF_SENSE_A. 
J13     2-3      Selects I_SENSE_DCB. 
J14     2-3      Selects V_SENSE_DCB_HALF. 

TWR-KE18F jumper settings: 
Jumper  Setting  Function 
J22     1-2      TWR System 5V Supply
J16     1-2      Power from OpenSDA USB
J17     1-2      VREFH - 3.3V
J18     1-2      Enable VDDA
J19     1-2      MCU VDD
J24     1-2      8MHz Clock
J11     1-2      SWD_CLK_TRGTMCU
J12     1-2      SWD_DIO_TGTMCU
J13     1-2      RST_TGTMCU_B
J4      1-2      Reset option - Default
J3      2-3      OpenSDA UART_TX
J5      2-3      OpenSDA UART_RX
J6      1-2      PTE5/CAN0_TX
J7      1-2      PTE4/CAN0_RX
J9      1-2      Enable Potentiometer

Prepare the Demo
===============
1.  Insert the TWR-KExxXxx MCU module and the TWR-MC-LV3PH module into the 
    TWR-ELEV card slots. Ensure that the primary sides of the modules (marked by white stripes) 
    are inserted into the primary elevator card (marked by white connectors). 
2.  After assembling the Tower System, connect the required cables as follows: 
    •  Connect the power input cable (3-wire connector) of the Linix motor to its corresponding 
       connector (J5) on the TWR-MC-LV3PH motor control driver board. 
    •  Plug in the power supply cable that is attached to the TWR-MC-LV3PH system kit to the 
       motor control peripheral board TWR-MC-LV3PH. 
    •  Connect the TWR MCU module to the host PC via a USB cable connected to J2 of 
       the TWR-KE18F MCU module and any USB port on the host PC. 

Running the demo
================
Press button SW2 on TWR-KE18F board to switch on or off demonstration mode - the motor will rotate. 

More information
================
For more information open the user guide in KSDK install folder\docs\MC\BLDCDEMOUG.pdf or visit the page: http://www.nxp.com/motorcontrol_bldc and find the doccument BLDCAPPACUG - 3-Phase Brushless DC (BLDC) Motor control User Guides.
