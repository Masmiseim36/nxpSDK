# power_manager_test_bm

## Overview
The power manager test application demonstrates the basic usage of power manager framework without RTOS.
The demo tests all features of power manager framework, including notification manager, wakeup source manager and so on.
The demo shows how the various power mode switch to each other based on power manager framework. However, in actual low power use case, to save energy and reduce the consumption even more, many things can be done including:
 - Disable the clock for unnecessary module during low power mode. That means, programmer can disable the clocks before entering the low power mode and re-enable them after exiting the low power mode when necessary.
 - Disable the function for unnecessary part of a module when other part would keep working in low power mode. At the most time, more powerful function means more power consumption. For example, disable the digital function for the unnecessary pin mux, and so on.
 - Set the proper pin state (direction and logic level) according to the actual application hardware. Otherwise, the pin current would be activated unexpectedly waste some energy.
 - Other low power consideration based on the actual application hardware.
 - Debug pins(e.g SWD_DIO) would consume additional power, had better to disable related pins or disconnect them.

## Supported Boards
- [MIMXRT1170-EVKB](../../../_boards/evkbmimxrt1170/demo_apps/power_manager_test/bm/example_board_readme.md)
- [EVK-MIMXRT595](../../../_boards/evkmimxrt595/demo_apps/power_manager_test/bm/example_board_readme.md)
- [EVK-MIMXRT685](../../../_boards/evkmimxrt685/demo_apps/power_manager_test/bm/example_board_readme.md)
- [FRDM-MCXN947](../../../_boards/frdmmcxn947/demo_apps/power_manager_test/bm/example_board_readme.md)
- [FRDM-MCXW71](../../../_boards/frdmmcxw71/demo_apps/power_manager_test/bm/example_board_readme.md)
- [FRDM-RW612](../../../_boards/frdmrw612/demo_apps/power_manager_test/bm/example_board_readme.md)
- [K32W148-EVK](../../../_boards/k32w148evk/demo_apps/power_manager_test/bm/example_board_readme.md)
- [KW45B41Z-EVK](../../../_boards/kw45b41zevk/demo_apps/power_manager_test/bm/example_board_readme.md)
- [KW47-EVK](../../../_boards/kw47evk/demo_apps/power_manager_test/bm/example_board_readme.md)
- [MCX-N9XX-EVK](../../../_boards/mcxn9xxevk/demo_apps/power_manager_test/bm/example_board_readme.md)
- [MCX-W72-EVK](../../../_boards/mcxw72evk/demo_apps/power_manager_test/bm/example_board_readme.md)
- [RD-RW612-BGA](../../../_boards/rdrw612bga/demo_apps/power_manager_test/bm/example_board_readme.md)
