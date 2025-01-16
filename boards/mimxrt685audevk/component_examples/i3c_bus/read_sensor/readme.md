# i3c_bus_master_read_icm42688p

## Overview
The example shows how to use i3c_bus component and create bus master to read on board sensor ICM-42688P:

The example will create i3c bus firstly, then create bus master which will do dynamic address assignment to the ICM-42688P sensor. Then reset the sensor for configuration, after that re-assgin sensor address via SETDASA CCC command because address is lost after reset. Then enable the ACCEL, GYRO function on sensor. Configure the sensor to generate IBI for TAP detect, then the application continuously read FIFO to retrieve the ACCEL and GYRO data, in the process if you tap the sensor on board, the IBI will generate master handles the IBI, print the IBI data.

## Supported Boards
- [MIMXRT685-AUD-EVK](../../../_boards/mimxrt685audevk/component_examples/i3c_bus/read_sensor/example_board_readme.md)
