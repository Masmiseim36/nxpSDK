Overview
========
This example demonstrates how to use the Simple Open EtherCAT Master (SOEM) Library to communicate with EhterCAT devices.

In this example there are three EhterCAT devices, one EtherCAT Coupler EK1100, one EtherCAT Terminal EL2008 (slave0),
and one EtherCAT Terminal EL1018 (slave1).

This example controls a stepper motor system using two outputs and one input remote IO:
  Outputs:
    -Pluse: The pulse signal with the period of 200us and the duty cycle of 50%.
    -Dir:  The direction signal.
  Inputs:
    -Limit signal: This signal comes from the Limit Switchs on the stepper motor system.
     The Dir signal changes once when one of the Limit Switchs has been touched.


Toolchain supported
===================
- IAR embedded Workbench  9.32.1
- Keil MDK  5.37
- GCC ARM Embedded  10.3.1
- MCUXpresso  11.7.0

Hardware requirements
=====================
- Mini/micro USB cable
- Network cable RJ45 standard
- MIMXRT1170-EVK board
- One BECKHOFF EK1100 EtherCAT Coupler
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Power up the EtherCAT Coupler and connect it to the target board via an Ethernet Cable.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Write the program to the flash of the target board.
5.  Press the reset button on your board to start the demo.

Running the demo
================
If the test passes, the led2 of slave0 lights up at 50% brightness.

If the stepper motor system is not setup properly, you can get information from
the console log and the led4 status of slave0.

When the demo is running, the serial port will output:


Start the soem_gpio_pulse baremetal example...

ec_init on enet0 succeeded.

ec_config_init 0
3 slaves found and configured.

ec_config_map_group IOmap:80000089 group:0
 >Slave 1, configadr 1001, state 12
 >Slave 2, configadr 1002, state 12
 >Slave 3, configadr 1003, state 12
  SII Isize:0
  SII Osize:0
     ISIZE:0 0 OSIZE:0
  SM programming
  SII Isize:0
  SII Osize:8
    SM0 length 8
     ISIZE:0 0 OSIZE:8
  SM programming
    SM0 Type:3 StartAddr: f00 Flags:   90044
  SII Isize:8
    SM0 length 8
  SII Osize:0
     ISIZE:8 8 OSIZE:0
  SM programming
    SM0 Type:4 StartAddr:1000 Flags:   10000
  OUTPUT MAPPING
    FMMU 0
      SM0
    slave 2 Outputs 80000089 startbit 0
 =Slave 3, INPUT MAPPING
    FMMU 0
      SM0
    Inputs 8000008A startbit 0
IOmapSize 2
Slaves mapped, state to SAFE_OP.

oloop = 1, iloop = 1

segments : 1 : 2 0 0 0

Request operational state for all slaves

Calculated workcounter 3

Operational state reached for all slaves.

