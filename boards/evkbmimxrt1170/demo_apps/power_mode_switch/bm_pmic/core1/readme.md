# power_mode_switch_bm_pmic_rt1xxx_core

## Overview
The Power mode switch bm pmic demo application demonstrates the usage of chip's power features with integration of 
PF5020. 
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

In chip PMIC board, DCDC is not used. PF5020's SW1 output(1.1V) is connected to chip's VDD_SOC_IN and SW2 output(1.8V)
is connected to chip's  VDD_1P8_IN. PF5020's 'STANDBY' pin is connected to PMIC_STBY_REQ.

Constraints with PMIC:
    1.If the chip needs to enter STBY mode, just call the API and there is no more difference between internal DCDC and external PMIC.
    2.If the chip doesn’t enter STBY mode, but will enter a low power mode, please pay attention to the DCDC output voltage of Current SP, Target SP and    Wakeup SP. These voltage will be referred DCDC_C, DCDC_T and DCDC_W for short.
    3.There are some constraints need to be met. 

Low power mode without STBY – Enter a low power mode:
    1.If DCDC_T > DCDC_C, increase DCDC voltage, switch SP, CPU mode switch
    2.If DCDC_T < DCDC_C, switch SP, then decrease DCDC voltage, CPU mode switch
    3.If DCDC_T = DCDC_C, switch SP, CPU mode switch

Low power mode without STBY – Wake up from a low power mode:
    1.If DCDC_W > DCDC_T, CPU wake up, increase the DCDC voltage, switch SP
    2.If DCDC_W < DCDC_T, CPU wake up, switch SP, decrease DCDC voltage
    3.If DCDC_W = DCDC_T, CPU wake up, switch SP

Notes:
1. PMIC is controlled by LPI2C6, to make sure the clock supply for it. Using RC16M as the clock source and make sure the clock gate is enabled under any power mode.
2. No matter CM7 or CM4 as the main core, a synchronization mechanism are needed. The main core here is used to control the PMIC through LPI2C6, so that make sure the main core has been completed the steps that the lower power mode without STBY, then the other can enter or apply to it owns low power mode.




Tips:
 This demo is to show how the various power mode can switch to each other. However, in actual low power use case, to save energy and reduce the consumption even more, many things can be done including:
 - Disable the clock for unnecessary module during low power mode. That means, programmer can disable the clocks before entering the low power mode and re-enable them after exiting the low power mode when necessary.
 - Disable the function for unnecessary part of a module when other part would keep working in low power mode. At the most time, more powerful function means more power consumption. For example, disable the digital function for the unnecessary pin mux, and so on.
 - Set the proper pin state (direction and logic level) according to the actual application hardware. Otherwise, the pin current would be activated unexpectedly waste some energy.
 - Other low power consideration based on the actual application hardware.
 - Debug pins(e.g SWD_DIO) would consume addtional power, had better to disable related pins or disconnect them.

## Supported Boards
- [MIMXRT1170-EVKB](../../../../_boards/evkbmimxrt1170/demo_apps/power_mode_switch/bm_pmic/example_board_readme.md)
