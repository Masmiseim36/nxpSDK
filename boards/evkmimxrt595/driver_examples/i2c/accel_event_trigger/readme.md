# i2c_accel_event_trigger

## Overview
This example demonstrate how to wake up main device in low power mode with accelerometer trigger event.
The accelerometer can keep working while the main device is in low power mode (or deep sleep mode).
Only when the configured event was captured, the accelerometer will trigger the interrupt to wake up
the main device.
This example uses I2C to configure the accelerometer to work in 800Hz data rate with low noise mode.
And when the tap event is triggered, it will wake up the main device. And 32 samples around the trigger
event will be captured.

## Supported Boards
- [EVK-MIMXRT595](../../../_boards/evkmimxrt595/driver_examples/i2c/accel_event_trigger/example_board_readme.md)
- [EVK-MIMXRT685](../../../_boards/evkmimxrt685/driver_examples/i2c/accel_event_trigger/example_board_readme.md)
