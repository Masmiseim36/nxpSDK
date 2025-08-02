# soem_servo_motor_bm

## Overview
This example demonstrates how to use the Simple Open EtherCAT Master (SOEM) Library to control servo-motor.

In this example there are 2 servo-motor:
	DELTA_ASDA_B3  * 1
	INOVANCE_SV680 * 1


## Running the demo
When the demo is running, the serial port will output:

NETC EP frame loopback example start.
Starting motion task
ec_init on netc0 succeeded.
ec_config_init 0
...
2 slaves found and configured.
ec_config_map_group IOmap:20000664 group:0
>Slave 1, configadr 1001, state  2
...
SM programming
SM2 Type:3 
StartAddr:1200 Flags:   10064
...
Slaves mapped, state to SAFE_OP.
Request operational state for all slaves
Calculated workcounter 6
Request operational state for all slaves
Calculated workcounter 6
Operational state reached for all slaves.


User can connect the oscilloscope to R35 register to observe processing time and jitter.
By raising the pin of the LED light before sending the Ethercat packet and lowering the pin of the LED light after sending the Ethercat packet. Measure the interval time using an oscilloscope to evaluate SOEM performance.
Note: Detailed SOEM performance please refter to EtherCAT Benchamrk.

## Supported Boards
- [MIMXRT1180-EVK](../../../_boards/evkmimxrt1180/soem_examples/soem_servo_motor/bm/example_board_readme.md)
- [IMX943-EVK](../../../_boards/imx943evk/soem_examples/soem_servo_motor/bm/example_board_readme.md)
