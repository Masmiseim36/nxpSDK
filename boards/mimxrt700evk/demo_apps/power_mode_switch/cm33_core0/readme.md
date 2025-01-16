# power_mode_switch

## Overview
This demo is used to show all the power modes in core 0 (the primary core in dual_core SoC). With this demo, user can navigate all the power modes that are existing in core 0.
In typical use case, the primary core is running the main control logic, while the secondary core is used to run the protocol or other complex mid-ware independently.
Then the secondary core prepares the resource (data, message, etc) into the somewhere, so that the primary core would fetch when necessary. In some other cases, primary core would send the some kinds of message to the secondary core, and ask the secondary core to enter indicated power mode. This demo download the image to both core, also provides the reference code to enter any power mode.

## Supported Boards
- [FRDM-K32L3A6](../../_boards/frdmk32l3a6/demo_apps/power_mode_switch/example_board_readme.md)
- [MIMXRT700-EVK](../../_boards/mimxrt700evk/demo_apps/power_mode_switch/example_board_readme.md)
