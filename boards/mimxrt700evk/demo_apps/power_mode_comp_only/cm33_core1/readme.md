# power_mode_comp_only

## Overview
The power_mode_switch application shows the usage of normal power mode control
APIs for entering the low power mode: Sleep mode, Deep Sleep mode, Deep Sleep Retention
mode, Deep Power Down mode and Full Deep Power Down mode. When the application
runs to each low powermode, the device would cut off the power for specific
modules to save energy. The device can be also waken up by prepared wakeup
source from external event. Tips: This demo is to show how the various power
mode can switch to each other. However, in actual low power use case, to save
energy and reduce the consumption even more, many things can be done including:

- Disable the clock for unnecessary module during low power mode. That means,
programmer can disable the clocks before entering the low power mode and re-enable
them after exiting the low power mode when necessary.

- Disable the function for unnecessary part of a module when other part would keep working in low
power mode. At the most time, more powerful function means more power consumption.
For example, disable the digital function for the unnecessary pin mux, and
so on.

- Set the proper pin state (direction and logic level) according to
the actual application hardware. Otherwise, the pin cirrent would be activied
unexpectedly waste some energy.

- Other low power consideration based on the actual application hardware.

- In order to meet typedef power consumption of DateSheet manual.
Please configure MCU under the following conditions.
1. Configure all pins as GPIO with pull-up resistor disabled in the IOCON
block.
2. Configure GPIO pins as outputs using the GPIO DIR register.
3. Write 1 to the GPIO CLR register to drive the outputs LOW.
4. All peripherals disabled.

## Supported Boards
- [MIMXRT700-EVK](../../_boards/mimxrt700evk/demo_apps/power_mode_comp_only/example_board_readme.md)
