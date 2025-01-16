# ecat_servo_motor

## Overview
This example demonstrates how to do motor control via EtherCAT on TwinCAT.

This example uses TwinCAT as EtherCAT Master, RT1180 as EtherCAT Slave, controls the motor through PMSM 
and uses EtherCAT CIA402 protocal for data transmission to control the motor in real time.

Note that the demo CAN NOT be built directly. Please see the below for more details.

## Running the demo
When the example is running, the serial port will output:

       Start the SSC servo_motor example...

Enter the position or speed on TwinCAT, and the motor will run according to the corresponding instructions.

NOTE: refer to the User Guide for the details of how to setup the PLC project on TwinCAT3.

## Supported Boards
- [MIMXRT1180-EVK](../../_boards/evkmimxrt1180/ecat_examples/servo_motor/example_board_readme.md)
