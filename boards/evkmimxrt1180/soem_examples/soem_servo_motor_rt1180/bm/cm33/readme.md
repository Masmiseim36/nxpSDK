# soem_servo_motor_rt1180_bm

## Overview
This example demonstrates how to use the Simple Open EtherCAT Master (SOEM) Library to control motor.

In this example there are 2 NXP EVK board:
	SOEM as EtherCAT MainDevice running on EVK board(i.MXRT1180/i.MX943)
	ecat_servo_motor example as EtherCAT SubDevice running on i.MXRT1180 EVK board
	Note: 
		ecat_servo_motor path: .../boards/evkmimxrt1180/ecat_examples/servo_motor
		ecat_servo_motor User Guide path: .../docs/ecatServoMotor


## Running the demo
If the test passes, the motor will start to retate.

When the demo is running, the serial port will output:

NETC EP frame loopback example start.
Starting motion task
ec_init on netc0 succeeded.
ec_config_init 0
...
1 slaves found and configured.
ec_config_map_group IOmap:20000664 group:0
>Slave 1, configadr 1001, state  2
...
Slaves mapped, state to SAFE_OP.
Request operational state for all slaves
Calculated workcounter 3
Request operational state for all slaves
Calculated workcounter 3
Operational state reached for all slaves.


Note: Detailed SOEM performance please refter to EtherCAT Benchamrk.

## Supported Boards
- [MIMXRT1180-EVK](../../../_boards/evkmimxrt1180/soem_examples/soem_servo_motor_rt1180/bm/example_board_readme.md)
- [IMX943-EVK](../../../_boards/imx943evk/soem_examples/soem_servo_motor_rt1180/bm/example_board_readme.md)
