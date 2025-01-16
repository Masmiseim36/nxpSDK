# power_manager_test_asymmetric_multiCore_bm_core

## Overview
This example demonstrates the basic usage of the Power Manager framework for asymmetric multicore platforms.
For such platforms, each one should use a copy of the Power Manager framework. Each copy of PM calculates the suitable
power state for each core based on its constraints. The primary core is responsible for setting clock roots,
configuring system pin mux, getting the target power state and timeout value from terminal, and using the Messaging
Unit (MU) module to send the selected power state and timeout value from the primary core to the secondary core.
The current consumption of each power state is aligned with the data provided in the DS.
The demo shows how the various power mode switch to each other based on power manager framework. However, in actual low power use case, to save energy and reduce the consumption even more, many things can be done including:
 - Disable the clock for unnecessary module during low power mode. That means, programmer can disable the clocks before entering the low power mode and re-enable them after exiting the low power mode when necessary.
 - Disable the function for unnecessary part of a module when other part would keep working in low power mode. At the most time, more powerful function means more power consumption. For example, disable the digital function for the unnecessary pin mux, and so on.
 - Set the proper pin state (direction and logic level) according to the actual application hardware. Otherwise, the pin current would be activated unexpectedly waste some energy.
 - Other low power consideration based on the actual application hardware.
 - Debug pins(e.g SWD_DIO) would consume additional power, had better to disable related pins or disconnect them.

## Supported Boards
- [MIMXRT1170-EVKB](../../../_boards/evkbmimxrt1170/demo_apps/power_manager_test/multiCore_bm/example_board_readme.md)
