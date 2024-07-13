Overview
========
This example demonstrates how to use the Simple Open EtherCAT Master (SOEM) Library to control servo-motor.

In this example there are 2 servo-motor:
	DELTA_ASDA_B3  * 1
	INOVANCE_SV680 * 1



SDK version
===========
- Version: 2.16.000

Toolchain supported
===================
- IAR embedded Workbench  9.60.1
- Keil MDK  5.39.0
- GCC ARM Embedded  13.2.1
- MCUXpresso  11.10.0

Hardware requirements
=====================
- Mini/micro USB cable
- Network cable RJ45 standard
- MIMXRT1180-EVK board
- Personal Computer
- Servo-Motor
	DELTA_ASDA_B3  * 1
	INOVANCE_SV680 * 1

Board settings
==============
No special settings are required.

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Power up the servo-motor and connect it to the target board(ENET4) via an Ethernet Cable.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Write the program to the flash of the target board.
5.  Start the demo and test SOEM performance.

Running the demo
================
If the test passes, D6 LED light will light up and the motor will start to retate.

When the demo is running, the serial port will output:

NETC EP frame loopback example start.
Starting motion task
ec_init on EtherCAT0 succeeded.
ec_config_init 0
Copy SII slave 2 from 1.
Copy SII slave 3 from 1.
...
31 slaves found and configured.
ec_config_map_group IOmap:20000664 group:0
>Slave 1, configadr 1001, state  2
CoE Osize:56 Isize:88
...
SM programming
SM2 Type:3 
StartAddr:1200 Flags:   10064
...
IOmapSize 558
Slaves mapped, state to SAFE_OP.
segments : 1 : 558 0 0 0
Request operational state for all slaves
Calculated workcounter 93
Request operational state for all slaves
Calculated workcounter 93
Operational state reached for all slaves.


User can connect the oscilloscope to R35 register to observe processing time and jitter.
By raising the pin of the LED light before sending the Ethercat packet and lowering the pin of the LED light after sending the Ethercat packet. Measure the interval time using an oscilloscope to evaluate SOEM performance.
Note: Detailed SOEM performance please refter to EtherCAT Benchamrk.
