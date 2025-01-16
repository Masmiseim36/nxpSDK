# power_mode_switch_bm_dcdc_rt1xxx_core

## Overview
The Power mode switch demo application demonstrates the usage of power modes.
The CPU mode is power mode of CPU platform. Each CPU platform has its own power mode. They are RUN, WAIT, STOP, SUSPEND.
Some resource that are managed at system level, and are not owned and controlled by any of the CPU platform are called as public resources.
Set point is used to control the power state of the public resources. With CPU Mode and Set Point defined properly,
the Power Mode of a system is defined as a combination of the CPU Mode and Set Point.
The Standby mode is the third kind of low power mode besides CPU mode and set point,
it is related to state of all CPU platform and has a much shorter transition time than setpoint.
Only when all CPU platforms send standby request can the system can enter into standby mode.

This demo prints the power mode menu through the debug console, where the user can set the MCU to a specific power mode.
User can wakeup the core by key interrupt. The purpose of this demo is to show how to switch between different power modes,
and how to configure a wakeup source and wakeup the MCU from low power modes.

 Tips:
 This demo is to show how the various power mode can switch to each other. However, in actual low power use case, to save energy and reduce the consumption even more, many things can be done including:
 - Disable the clock for unnecessary module during low power mode. That means, programmer can disable the clocks before entering the low power mode and re-enable them after exiting the low power mode when necessary.
 - Disable the function for unnecessary part of a module when other part would keep working in low power mode. At the most time, more powerful function means more power consumption. For example, disable the digital function for the unnecessary pin mux, and so on.
 - Set the proper pin state (direction and logic level) according to the actual application hardware. Otherwise, the pin current would be activated unexpectedly waste some energy.
 - Other low power consideration based on the actual application hardware.
 - Debug pins(e.g SWD_DIO) would consume additional power, had better to disable related pins or disconnect them.

## Supported Boards
- [MIMXRT1160-EVK](../../../../_boards/evkmimxrt1160/demo_apps/power_mode_switch/bm_dcdc/example_board_readme.md)
