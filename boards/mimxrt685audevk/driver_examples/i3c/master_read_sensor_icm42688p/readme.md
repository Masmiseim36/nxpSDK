# i3c_master_read_sensor_icm42688p

## Overview
The example shows how to use i3c driver as master to read on board sensor ICM-42688P.

The example will do dynamic address assignment to the ICM-42688P sensor and then reset
the sensor for configuration, after that re-do address assign because of address lost.
Then enable the ACCEL, GYRO function on sensor. Configure the sensor to generate IBI for
TAP detect, then the application continuously read FIFO to retrieve the ACCEL and GYRO
data, in the process if you tap the sensor on board, the IBI will generate master handles
the IBI, print the IBI data.

## Supported Boards
- [LPCXpresso55S36](../../../_boards/lpcxpresso55s36/driver_examples/i3c/master_read_sensor_icm42688p/example_board_readme.md)
- [LPCXpresso860MAX](../../../_boards/lpcxpresso860max/driver_examples/i3c/master_read_sensor_icm42688p/example_board_readme.md)
- [MIMXRT685-AUD-EVK](../../../_boards/mimxrt685audevk/driver_examples/i3c/master_read_sensor_icm42688p/example_board_readme.md)
